#ifdef __GNUG__
#  pragma implementation
#endif

#include <list>
#include <sstream>

#include "config.h"
#include <futil/WmlDbg.h>
#include <futil/Process.h>

#include "FilterFactory.h"
#include "Filter.h"
#include "Backend.h"

using namespace std;
using namespace dt;

int FilterFactory::returnCode = -1;

unique_ptr<BaseFilter>
FilterFactory::create (const std::string& path)
{
        // To determine if this is a standard CUPS filter or an
        // extended DT filter, call the filter with a DT filter
        // specific argument that returns a list of supported
        // features. If this works then it is a DT filter (with the
        // specified feature set), otherwise it's a standard CUPS
        // filter.

        list<string> args;
        args.push_back ("dummy"); // NB: The first item in the args list is discarded
        args.push_back ("--list-features");

        wml::Process p;
        unique_ptr<BaseFilter> pf;
        try {
                FilterFactory::runProcess (p, path, args);
        } catch (const exception& e) {
                // Exception thrown
                DBG (e.what());
        }
        if (FilterFactory::returnCode > 0) {
                // We expect a CUPS filter program to return a
                // non-zero value when called with an unexpected
                // number of arguments.
                DBG ("Non-zero return code: creating a standard CUPS filter object");
                pf.reset (new BaseFilter (path));
        } else {
                DBG ("Filter call succeeded: creating a DT filter object");
                pf.reset (new Filter (path));
                // Set up filter features.
                string features (p.readAllStandardOutput());
                DBG ("Setting features: " << features);
                pf->setFeatures (features);
        }

        return pf;
}

unique_ptr<Backend>
FilterFactory::createBackend (const std::string& path)
{
        // To determine if this is a standard CUPS backend or an
        // extended DT backend, call the filter with a DT specific
        // argument. If this works then it is a DT backend, otherwise
        // it's a standard CUPS backend.

        list<string> args;
        args.push_back ("dummy"); // NB: The first item in the args list is discarded
        args.push_back ("--list-features"); // Dummy argument in this case

        wml::Process p;
        unique_ptr<Backend> pb;
        try {
                FilterFactory::runProcess (p, path, args);
        } catch (const exception& e) {
                // Exception thrown
                stringstream err;
                err << "Caught an error: " << e.what();
                DBG (err.str());
        }

        if (FilterFactory::returnCode > 0) {
                // We expect a CUPS backend to return a non-zero value
                // when called with an unexpected number of arguments.
                DBG ("Non-zero return code: creating a standard CUPS backend object");
                pb.reset (new Backend (path));
        } else {
                DBG ("Backend call succeeded: creating a DT backend object");
                pb.reset (new BackendPlus (path));
        }

        return pb;
}

void
FilterFactory::runProcess (wml::Process& p, const string& path, const list<string>& args)
{
        // Initialize return code, in anticipation of storing the
        // result of calling the filter.
        FilterFactory::returnCode = -1;

        FilterFactoryCallbacks cb;
        p.setCallbacks (&cb);
        p.start (path, args);
        p.probeProcess();
        if (!p.waitForStarted()) {
                // Process failed to start
                throw runtime_error ("Process failed to start");
        }
        while (p.running() == true) {
                p.probeProcess();
        }
        string err (p.readAllStandardError());
        if (!err.empty()) {
                throw runtime_error (err);
        }
}

void
FilterFactory::storeReturnCode (int i)
{
        FilterFactory::returnCode = i;
}

/*
 * FilterFactoryCallbacks implementation.
 */

FilterFactoryCallbacks::FilterFactoryCallbacks (void)
        : ProcessCallbacks()
{
        using namespace std::placeholders; // for `_1`
        this->addReturnCodeSignalCallback (bind(&FilterFactory::storeReturnCode, _1));
}
