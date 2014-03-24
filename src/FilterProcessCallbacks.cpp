#ifdef __GNUG__
#  pragma implementation
#endif

#include "config.h"
#include <futil/WmlDbg.h>

#include "FilterProcessCallbacks.h"

using namespace std;
using namespace dt;

/*
 * FilterProcessCallbacks implementation.
 */

FilterProcessCallbacks::FilterProcessCallbacks (Datastream* p)
        : ProcessCallbacks()
{
        this->parent = p;
}

FilterProcessCallbacks::~FilterProcessCallbacks (void)
{
}

void
FilterProcessCallbacks::startedSignal (string msg)
{
        DBG ("Called");
}

void
FilterProcessCallbacks::errorSignal (int err)
{
        DBG ("Called for error: " << err);
}

void
FilterProcessCallbacks::processFinishedSignal (string msg)
{
        DBG ("Called");
        //this->parent->setFilterProcessFinishedMessage (msg);
}

void
FilterProcessCallbacks::readyReadStandardOutputSignal (void)
{
        DBG ("Called");
        //this->parent->filterStdoutReady();
}

void
FilterProcessCallbacks::readyReadStandardErrorSignal (void)
{
        DBG ("Called");
        //this->parent->filterStderrReady();
}
