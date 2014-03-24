#ifdef __GNUG__
#  pragma implementation
#endif

#include <sstream>

#include "config.h"
#include <futil/WmlDbg.h>

#include "Datastream.h"
#include "Filter.h"
#include "FilterProcessCallbacks.h"

using namespace std;
using namespace dt;
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

        // Do we need callbacks for the filter process?
        FilterProcessCallbacks cb (this);
        this->p.setCallbacks (&cb);

        list<string> args;
        args.push_back (this->name);
        args.push_back (data.getId());
        args.push_back ("dsuser"); //data.getUser());
        args.push_back ("data"); // data.getTitle());
        args.push_back ("1"); // data.getCopies());
        args.push_back ("0"); // data.getOptions());
        args.push_back ("/tmp/" + data.getId()); // data.getOptions());

        // This doesn't read properly in the filter program:
        // string ifp ("/tmp/" + data.getId());
        // freopen (ifp.c_str(), "r", stdin);

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

                // Write previous filter output to process stdin.
                if (!this->filterOutput.empty()) {
                        this->p.writeIn (this->filterOutput);
                }

                while (this->p.running()) {
                        usleep (1000);
                        this->p.probeProcess();
                }

                // Do we need to save off standard output every time a
                // 'standard output ready' signal is received?
                this->filterOutput = this->p.readAllStandardOutput();
                gotOutput = !this->filterOutput.empty();

                if (gotOutput) {
                        string::size_type pos (path.rfind("/"));
                        string tfp ("/tmp/" + data.getId());
                        tfp += "_" + path.substr(pos+1);

                        ofstream fout (tfp.c_str());
                        fout << this->filterOutput;
                        fout.close();

                        DBG ("Wrote filter output to " << tfp);

                        // Replace original file path arg with new version
                        args.pop_back();
                        args.push_back (tfp);

                } else {
                        DBG ("No output from filter " << path);
                }

                string err (this->p.readAllStandardError());
                if (!err.empty()) {
                        stringstream ee;
                        ee << "The filter '" << f.getPath()
                           << "' returned the following on stderr: '" << err << "'";
                        throw runtime_error (ee.str());
                }

                ++i;
        }

        if (!gotOutput) {
                // No output from filter chain
                // DBG ("No output from filter '" << *this->lastFilter << "'");
        }

        if (autoFilterMode) {
                this->filters.clear();
        }
}
