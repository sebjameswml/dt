#ifdef __GNUG__
#  pragma implementation
#endif

#include <sstream>
#include <fstream>

#include "config.h"
#include <futil/WmlDbg.h>

#include "src/log/Event.h"
#include "src/log/SyslogStorage.h"
#include "src/log/MySQLStorage.h"

#include "src/dtmime/mime.h"
#include "src/dtcups/cups-private.h"
#include "src/dtppd/ppd.h"

#include "Datastream.h"
#include "Filter.h"
#include "Backend.h"
#include "FilterFactory.h"

using namespace std;
using namespace dt;
using namespace dt::log;

const string
Datastream::defaultFilterPath = "/usr/lib/dt/filter:/usr/lib/cups/filter";

Datastream::Datastream()
        : id ("")
        , name ("")
        , filterPath (Datastream::defaultFilterPath)
{
}

Datastream::Datastream (const string& theId)
        : id (theId)
        , name ("")
        , filterPath (Datastream::defaultFilterPath)
{
        this->initialise();
}

Datastream::~Datastream()
{
}

//
// Methods
//
void
Datastream::initialise (void)
{
        // Check for settings for this ID
        this->settings.initialise ("/etc/wml/pp/datastream/" + this->id);
        if (this->settings.ready()) {
                // Set up datastream from settings
                this->name = this->settings.getSetting ("Datastream", "name");
                this->filters = this->settings.getListStringsSetting ("Datastream", "filters");
                this->filterPath = this->settings.getSetting ("Datastream", "filterPath");
        }
}

void
Datastream::write (void)
{
        this->settings.setSetting ("Datastream", "id", this->id);
        this->settings.setSetting ("Datastream", "name", this->name);
        this->settings.setSetting ("Datastream", "filters", this->filters);
        this->settings.setSetting ("Datastream", "filterPath", this->filterPath);
        this->settings.write();
}

void
Datastream::process (Data& data)
{
        // Base 'filter mode' on whether or not this->filters contains
        // entries.
        bool autoFilterMode (this->filters.empty());
        if (autoFilterMode) {
                // Identify the input data and set up the list of
                // filters based on mime.types and mime.convs, as in
                // CUPS.
                this->populateFilters (data);
        }

        // Set up callbacks for the filter process
        this->setupFilterProcessCallbacks();

        // Clear storage for stdout/stderr.
        this->filterOutput = "";
        this->filterError = "";

        ifstream from (data.getPath());
        if (from.is_open()) {
                char buf[64];
                while (!from.eof()) {
                        from.read (buf, 63);
                        this->filterOutput.append (buf, from.gcount());
                }
        } else {
                throw runtime_error ("Couldn't open input data file");
        }

        // Apply each filter in turn until either we come to the end
        // of the chain or one of the filters does not create any
        // further output.

        bool gotOutput (true);
        list<string> args;
        auto i (this->filters.begin()), end (this->filters.end());
        this->lastFilter = end;
        while (i != end && gotOutput) {

                this->lastFilter = i;

                // Use a factory to get a pointer to a BaseFilter,
                // which can be used to get the appropriate argument
                // list for the filter.
                unique_ptr<BaseFilter> pf (FilterFactory::create (*i));
                if (pf.get() == nullptr) {
                        throw runtime_error ("Error creating filter object");
                }

                string path (pf->getPath());

                pf->populateArgs (*this, data, args);

                // Apply the filter
                this->p.reset (true); // keepCallbacks = true
                this->p.start (path, args);
                if (!this->p.waitForStarted()) {
                        throw runtime_error ("Process didn't start");
                }

                if (!this->filterOutput.empty()) {
                        // Got output from previous filter, send this
                        // to process stdin.
                        this->p.writeIn (this->filterOutput);
                        this->p.closeWritingEnd();
                }

                // Clear storage for stdout/stderr.
                this->filterOutput = "";
                this->filterError = "";

                while (this->p.running()) {
                        usleep (1000);
                        this->p.probeProcess();
                }

                // Get any remaining data on stdout/stderr.
                this->filterStdoutReady();
                this->filterStderrReady();

                gotOutput = (!this->filterOutput.empty());
                if (gotOutput) {
                        // Write to the filter output file.
                        // TODO use DTDATA path (runtime configurable)
                        // and f.getName() (and/or count) for file path.
                        string tfp = "/tmp/" + data.getId()
                                + "-" + this->name
                                + "." + path.substr(path.rfind("/")+1);

                        fstream fstrm (tfp.c_str(), ios::out|ios::trunc);
                        fstrm << this->filterOutput;
                        fstrm.close();

                        data.setPath (tfp);

                        stringstream ss;
                        ss << "Wrote filter output to " << tfp;
                        this->logEvent (data, ss.str());

                        if (args.size() > 6) {
                                // Remove original file path arg -
                                // we'll pass data via stdin for
                                // subsequent filters.
                                args.pop_back();
                        }
                }

                if (!this->filterError.empty()) {
                        stringstream ee;
                        ee << "The filter '" << path
                           << "' returned the following on stderr: '" << this->filterError << "'";
                        this->logEvent (data, ee.str());
                        throw runtime_error (ee.str());
                }

                ++i;
        }

        if (gotOutput) {
                // Apply backend, if specified
                if (!this->backend.empty()) {

                        // Use a factory to get a pointer to a
                        // Backend, which can be used to get the
                        // appropriate argument list for the backend.
                        unique_ptr<Backend> pb (FilterFactory::createBackend (this->backend));
                        if (pb.get() == nullptr) {
                                throw runtime_error ("Error creating backend object");
                        }

                        string path (pb->getPath());

                        pb->populateArgs (*this, data, args);

                        // Apply the backend

                        this->p.reset (true); // keepCallbacks = true

                        this->p.start (path, args);
                        if (!this->p.waitForStarted()) {
                                throw runtime_error ("Process didn't start");
                        }

                        if (!this->filterOutput.empty()) {
                                // Send output from previous filter to
                                // process stdin.
                                this->p.writeIn (this->filterOutput);
                                this->p.closeWritingEnd();
                        }

                        // Clear storage for stdout/stderr.
                        this->filterOutput = "";
                        this->filterError = "";

                        while (this->p.running()) {
                                usleep (1000);
                                this->p.probeProcess();
                        }

                        // Get any remaining data on stdout/stderr.
                        this->filterStdoutReady();
                        this->filterStderrReady();

                        if (this->filterError.empty()) {
                                stringstream ss;
                                ss << "Applied backend " << this->backend;
                                this->logEvent (data, ss.str());
                        } else {
                                stringstream ee;
                                ee << "The backend '" << path
                                   << "' returned the following on stderr: '" << this->filterError << "'";
                                this->logEvent (data, ee.str());
                                throw runtime_error (ee.str());
                        }
                }

        } else {
                // No output from filter chain
                stringstream ss;
                ss << "No output from filter '" << *this->lastFilter << "'";
                this->logEvent (data, ss.str());
        }

        if (autoFilterMode) {
                this->filters.clear();
        }
}

void
Datastream::populateFilters (Data& data)
{
        char          super[MIME_MAX_SUPER],  /* Super-type name */
                      type[MIME_MAX_TYPE],    /* Type name */
                      program[1024],          /* Program/filter name */
                      path[1024];             /* Full path to filter program */

        int           compression;            /* Compression of file */
        int           total_cost,             /* Cost of filters */
                      prev_cost,              /* Previous cost */
                      cost;                   /* Cost of PPD filter */
        mime_t        *mime;                  /* MIME database */
        mime_type_t   *src,                   /* Source type */
                      *dst;                   /* Destination type */
        cups_array_t  *mime_filters;          /* Filters for the file */
        mime_filter_t *filter;                /* Current filter */
        ppd_file_t    *ppd;                   /* PPD file */
        ppd_attr_t    *attr;                  /* PPD attribute */

        mime        = NULL;
        src         = NULL;
        dst         = NULL;
        prev_cost   = 0;

        if (!mime) {
                mime = mimeLoad("/etc/dt/mime", this->filterPath.c_str());
        }

        string file (data.getPath().c_str());

        src = mimeFileType(mime, file.c_str(), NULL, &compression);

        if (src) {
                DBG (file << ": " << src->super << "/" << src->type
                     << (compression ? " (gzipped)" : ""));
        } else if ((src = mimeType(mime, "application", "octet-stream")) != NULL) {
                DBG (file << ": application/octet-stream");
        } else {
                DBG (file << ": unknown");
                if (mime)
                        mimeDelete(mime);
                return;
        }

        // Presumably we store the PPD name/path (or use a set path
        // based on datastream name).
        string ppdFile ("/etc/dt/ppd/");
        ppdFile += this->name + ".ppd";

        DBG ("About to ppdOpenFile(" << ppdFile << ")");
        ppd = ppdOpenFile (ppdFile.c_str());
        if (ppd == (ppd_file_t*)0) {
                DBG ("Error opening PPD file");
                mimeDelete(mime);
                return;
        }

        for (attr = ppdFindAttr (ppd, "cupsFilter", NULL);
             attr;
             attr = ppdFindNextAttr (ppd, "cupsFilter", NULL)) {

                DBG ("Got cupsFilter: " << attr->value);

                // *cupsFilter: "source/type cost program"
                if (!attr->value ||
                    sscanf(attr->value, "%15[^/]/%255s%d%*[ \t]%1023[^\n]", super, type,
                           &cost, program) != 4) {
                        throw runtime_error ("Error getting cupsFilter entry from PPD");
                }

                dst = mimeType(mime, super, type);

                mime_filters = mimeFilter(mime, src, dst, &total_cost);

                if (!mime_filters) {
                        DBG ("No filters to convert from " << src->super << "/"
                             << src->type << " to " << dst->super << "/" << dst->type);
                } else {
                        // Include cost of converting to printer's native format:
                        total_cost += cost;

                        if (prev_cost == 0 || total_cost < prev_cost) {

                                this->filters.clear();

                                filter = (mime_filter_t *)cupsArrayFirst(mime_filters);
                                while (filter) {
                                        if (cupsFileFind(filter->filter, this->filterPath.c_str(),
                                                         1, path, sizeof(path))) {
                                                this->filters.push_back (path);
                                        }
                                        filter = (mime_filter_t *)cupsArrayNext(mime_filters);
                                }
                                prev_cost = total_cost;
                        }
                        cupsArrayDelete(mime_filters);
                }
        }

        ppdClose (ppd);

        if (mime) {
                mimeDelete(mime);
        }
}

void
Datastream::setupFilterProcessCallbacks (void)
{
        this->cb.clear();
        // Using lambda expression (passing local variables by
        // reference) instead of the alternative form with std::bind.
        this->cb.addReadStdoutSignalCallback ([&]{ this->filterStdoutReady(); });
        this->cb.addReadStderrSignalCallback ([&]{ this->filterStderrReady(); });
        this->p.setCallbacks (&this->cb);
}

string
Datastream::getOption (const string& program,
                       const string& feature,
                       const string& option) const
{
        if (!this->settings.ready()) {
                throw runtime_error ("Datastream settings not initialised");
        }
        vector<pair<string, string> > groups = {
                { "Datastream", "" }
                , { program, "" } // ID? - filter could be applied more than once?
        };
        if (!feature.empty()) {
                groups.push_back ({ feature, "" });
        }
        return this->settings.getSetting (groups, option);
}

void
Datastream::setOption (const string& program,
                       const string& feature,
                       const string& option,
                       const string& value)
{
        if (!this->settings.ready()) {
                throw runtime_error ("Datastream settings not initialised");
        }
        vector<pair<string, string> > groups = {
                { "Datastream", "" }
                , { program, "" } // ID? - filter could be applied more than once?
        };
        if (!feature.empty()) {
                groups.push_back ({ feature, "" });
        }
        this->settings.setSetting (groups, option, value);
        this->settings.write();
}

void
Datastream::filterStdoutReady (void)
{
        this->filterOutput += this->p.readAllStandardOutput();
}

void
Datastream::filterStderrReady (void)
{
        this->filterError += this->p.readAllStandardError();
}

void
Datastream::logEvent (Data& data, const string& msg)
{
        DBG ("Logging event for data " << data.getId() << " on datastream "
             << this->name << ": " << msg);
#ifdef SWITCH_ON_LOGGING
        Event e;
        e.setDataId (data.getId());
        e.setDatastreamName (this->name);
        e.setDatastreamId (this->id);
        e.setMessage (msg);

        // For each storage type...
        MySQLStorage s;
        e.accept (s);
#endif
}

//
// Accessors
//

string
Datastream::getId (void) const
{
        return this->id;
}

void
Datastream::setId (const string& s)
{
        this->id = s;
}

string
Datastream::getName (void) const
{
        return this->name;
}

void
Datastream::setName (const string& s)
{
        this->name = s;
}

list<string>
Datastream::getFilters (void) const
{
        return this->filters;
}

void
Datastream::setFilters (const list<string>& f)
{
        this->filters = f;
}

string
Datastream::getFilterPath (void) const
{
        return this->filterPath;
}

void
Datastream::setFilterPath (const string& s)
{
        this->filterPath = s;
}

string
Datastream::getBackend (void) const
{
        return this->backend;
}

void
Datastream::setBackend (const string& s)
{
        this->backend = s;
}
