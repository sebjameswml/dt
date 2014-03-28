#ifdef __GNUG__
#  pragma implementation
#endif

#include <sstream>

#include "config.h"
#include <futil/WmlDbg.h>

#include "src/log/Event.h"
#include "src/log/SyslogStorage.h"
#include "src/log/MySQLStorage.h"

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
        }

        // Callbacks for the filter process
        FilterProcessCallbacks cb (this);
        this->p.setCallbacks (&cb);

        // Clear storage for stdout/stderr.
        this->filterOutput = "";
        this->filterError = "";

        list<string> args;
        args.push_back (this->name);
        args.push_back (data.getId());
        args.push_back (data.getUser());
        args.push_back (data.getName());
        args.push_back (data.getCopiesStr());
        args.push_back ("0"); // data.getOptions());
        args.push_back (data.getPath());

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
