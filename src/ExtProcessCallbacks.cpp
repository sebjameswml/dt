#ifdef __GNUG__
#  pragma implementation
#endif

#include "config.h"
#include <futil/WmlDbg.h>

#include "ExtProcessCallbacks.h"

using namespace std;
using namespace dt;

ExtProcessCallbacks::ExtProcessCallbacks()
{
}

ExtProcessCallbacks::~ExtProcessCallbacks()
{
}

void
ExtProcessCallbacks::startedSignal (string s)
{
        try {
                this->startedSignalCallback (s);
        } catch (const std::bad_function_call& e) {
                // No callback assigned
                DBG ("Process started: " << s);
        }
}

void
ExtProcessCallbacks::errorSignal (int i)
{
        try {
                this->errorSignalCallback (i);
        } catch (const std::bad_function_call& e) {
                // No callback assigned
                DBG ("Error: " << i);
        }
}

void
ExtProcessCallbacks::processFinishedSignal (string s)
{
        try {
                this->processFinishedSignalCallback (s);
        } catch (const std::bad_function_call& e) {
                // No callback assigned
                DBG ("Process finished: " << s);
        }
}

void
ExtProcessCallbacks::returnCodeSignal (int i)
{
        try {
                this->returnCodeSignalCallback (i);
        } catch (const std::bad_function_call& e) {
                // No callback assigned
                DBG ("Process returned: " << i);
        }
}

void
ExtProcessCallbacks::readyReadStandardOutputSignal (void)
{
        try {
                this->readStdoutSignalCallback();
        } catch (const std::bad_function_call& e) {
                // No callback assigned
                DBG ("Ready to read standard output");
        }
}
void
ExtProcessCallbacks::readyReadStandardErrorSignal (void)
{
        try {
                this->readStderrSignalCallback();
        } catch (const std::bad_function_call& e) {
                // No callback assigned
                DBG ("Ready to read standard error");
        }
}

void
ExtProcessCallbacks::addStartedSignalCallback (function<void(string)> cb)
{
        this->startedSignalCallback = cb;
}

void
ExtProcessCallbacks::addErrorSignalCallback (function<void(int)> cb)
{
        this->errorSignalCallback = cb;
}

void
ExtProcessCallbacks::addProcessFinishedSignalCallback (function<void(string)> cb)
{
        this->processFinishedSignalCallback = cb;
}

void
ExtProcessCallbacks::addReturnCodeSignalCallback (function<void(int)> cb)
{
        this->returnCodeSignalCallback = cb;
}

void
ExtProcessCallbacks::addReadStdoutSignalCallback (std::function<void()> cb)
{
        this->readStdoutSignalCallback = cb;
}

void
ExtProcessCallbacks::addReadStderrSignalCallback (std::function<void()> cb)
{
        this->readStderrSignalCallback = cb;
}
