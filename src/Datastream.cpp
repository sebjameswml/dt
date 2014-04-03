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

#include "Datastream.h"
#include "Filter.h"
#include "FilterProcessCallbacks.h"

using namespace std;
using namespace dt;
using namespace dt::log;
using namespace wml;

Datastream::Datastream()
        : id ("")
        , name ("")
{
}

Datastream::Datastream (const string& theId)
        : id (theId)
        , name ("")
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
        }
}

void
Datastream::write (void)
{
        this->settings.setSetting ("Datastream", "id", this->id);
        this->settings.setSetting ("Datastream", "name", this->name);
        this->settings.setSetting ("Datastream", "filters", this->filters);
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

        // Callbacks for the filter process
        FilterProcessCallbacks cb (this);
        this->p.setCallbacks (&cb);

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

        list<string> args;
        args.push_back (this->name);
        args.push_back (data.getId());
        args.push_back (data.getUser());
        args.push_back (data.getName());
        args.push_back (data.getCopiesStr());
        args.push_back ("0"); // data.getOptions());

        // Apply each filter in turn until either we come to the end
        // of the chain or one of the filters does not create any
        // further output.

        bool gotOutput (true);
        auto i (this->filters.begin()), end (this->filters.end());
        this->lastFilter = end;
        while (i != end && gotOutput) {

                this->lastFilter = i;

                Filter f (*i);

                // Apply the filter
                string path (f.getPath());

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

                        fstream f (tfp.c_str(), ios::out|ios::trunc);
                        f << this->filterOutput;
                        f.close();

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
                        ee << "The filter '" << f.getPath()
                           << "' returned the following on stderr: '" << this->filterError << "'";
                        this->logEvent (data, ee.str());
                        throw runtime_error (ee.str());
                }

                ++i;
        }

        if (!gotOutput) {
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
                      type[MIME_MAX_TYPE];    /* Type name */
        int           compression;            /* Compression of file */
        int           cost;                   /* Cost of filters */
        mime_t        *mime;                  /* MIME database */
        mime_type_t   *src,                   /* Source type */
                      *dst;                   /* Destination type */
        cups_array_t  *mime_filters;          /* Filters for the file */
        mime_filter_t *filter;                /* Current filter */

        mime        = NULL;
        src         = NULL;
        dst         = NULL;

        // TODO make this user configurable:
        string filterDir ("/usr/lib/cups/filter");

        if (!mime) {
                mime = mimeLoad("/etc/dt/mime", filterDir.c_str());
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

        // TODO destination file type is an aspect of the
        // datastream so this should be
        // this->getDestFileType() or whatever.
        string dest ("application/vnd.wml-pdf");

        sscanf(dest.c_str(), "%15[^/]/%31s", super, type);
        dst = mimeType(mime, super, type);

        mime_filters = mimeFilter(mime, src, dst, &cost);

        if (!mime_filters) {
                DBG ("No filters to convert from " << src->super << "/"
                     << src->type << " to " << dst->super << "/" << dst->type);
        } else {
                filterDir += "/";
                filter = (mime_filter_t *)cupsArrayFirst(mime_filters);
                this->filters.push_back (filterDir + filter->filter);

                for (filter = (mime_filter_t *)cupsArrayNext(mime_filters);
                     filter;
                     filter = (mime_filter_t *)cupsArrayNext(mime_filters)) {
                        this->filters.push_back (filterDir + filter->filter);
                }

                cupsArrayDelete(mime_filters);
        }

        if (mime) {
                mimeDelete(mime);
        }
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
