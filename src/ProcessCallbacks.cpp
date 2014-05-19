#ifdef __GNUG__
#  pragma implementation
#endif

#include "config.h"
#include <futil/WmlDbg.h>

#include "ProcessCallbacks.h"

using namespace std;
using namespace dt;

ProcessCallbacks::ProcessCallbacks()
{
}

ProcessCallbacks::~ProcessCallbacks()
{
}

void
ProcessCallbacks::startedSignal (string s)
{
        try {
                this->startedSignalCallback (s);
        } catch (const std::bad_function_call& e) {
                // No callback assigned
                DBG ("Process started: " << s);
        }
}

void
ProcessCallbacks::errorSignal (int i)
{
        try {
                this->errorSignalCallback (i);
        } catch (const std::bad_function_call& e) {
                // No callback assigned
                DBG ("Error: " << i);
        }
}

void
ProcessCallbacks::processFinishedSignal (string s)
{
        try {
                this->processFinishedSignalCallback (s);
        } catch (const std::bad_function_call& e) {
                // No callback assigned
                DBG ("Process finished: " << s);
        }
}

void
ProcessCallbacks::returnCodeSignal (int i)
{
        try {
                this->returnCodeSignalCallback (i);
        } catch (const std::bad_function_call& e) {
                // No callback assigned
                DBG ("Process returned: " << i);
        }
}

void
ProcessCallbacks::readyReadStandardOutputSignal (void)
{
        try {
                this->readStdoutSignalCallback();
        } catch (const std::bad_function_call& e) {
                // No callback assigned
                DBG ("Ready to read standard output");
        }
}
void
ProcessCallbacks::readyReadStandardErrorSignal (void)
{
        try {
                this->readStderrSignalCallback();
        } catch (const std::bad_function_call& e) {
                // No callback assigned
                DBG ("Ready to read standard error");
        }
}

void
ProcessCallbacks::addStartedSignalCallback (function<void(string)> cb)
{
        this->startedSignalCallback = cb;
}

void
ProcessCallbacks::addErrorSignalCallback (function<void(int)> cb)
{
        this->errorSignalCallback = cb;
}

void
ProcessCallbacks::addProcessFinishedSignalCallback (function<void(string)> cb)
{
        this->processFinishedSignalCallback = cb;
}

void
ProcessCallbacks::addReturnCodeSignalCallback (function<void(int)> cb)
{
        this->returnCodeSignalCallback = cb;
}

void
ProcessCallbacks::addReadStdoutSignalCallback (std::function<void()> cb)
{
        this->readStdoutSignalCallback = cb;
}

void
ProcessCallbacks::addReadStderrSignalCallback (std::function<void()> cb)
{
        this->readStderrSignalCallback = cb;
}

void
ProcessCallbacks::clear (void)
{
        this->startedSignalCallback = nullptr;
        this->errorSignalCallback = nullptr;
        this->returnCodeSignalCallback = nullptr;
        this->processFinishedSignalCallback = nullptr;
        this->readStdoutSignalCallback = nullptr;
        this->readStderrSignalCallback = nullptr;
}
