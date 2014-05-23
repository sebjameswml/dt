#ifdef __GNUG__
#  pragma implementation
#endif

#include <list>

#include "config.h"
#include <futil/WmlDbg.h>
#include <futil/Process.h>

#include "FilterFactory.h"
#include "Filter.h"

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

        unique_ptr<BaseFilter> pf;

        wml::Process p;
        list<string> args;
        args.push_back ("dummy"); // NB: The first item in the args list is discarded
        args.push_back ("--list-features");

        // Initialize return code, in anticipation of storing the
        // result of calling the filter.
        FilterFactory::returnCode = -1;

        FilterFactoryCallbacks cb;
        p.setCallbacks (&cb);
        p.start (path, args);
        p.probeProcess();
        if (!p.waitForStarted()) {
                // Process failed to start
                return pf;
        }
        while (p.running() == true) {
                p.probeProcess();
        }

        string err (p.readAllStandardError());
        if (!err.empty()) {
                throw runtime_error (err);
        } else if (FilterFactory::returnCode > 0) {
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
