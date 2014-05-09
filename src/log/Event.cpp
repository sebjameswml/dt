#ifdef __GNUG__
#  pragma implementation
#endif

#include <unistd.h>
#include <uuid/uuid.h>

#include <stdexcept>

#include "config.h"

#include "Event.h"

using namespace std;
using namespace dt;
using namespace dt::log;

Event::Event()
        : id("")
        , dataId("")
        , message("")
#ifdef GOT_DT_ERROR_CODE
        , errorCode() // dt::errorCode
#endif
        , datastreamId("")
        , datastreamName("")
        , hostname("")
#ifdef GOT_DT_LOG_LEVEL
        , level() // dt::logLevel
#endif
        , pid (getpid())
{
        // Initialise timeval struct
        gettimeofday (&time, NULL);
        this->generateId();
}

Event::~Event()
{
}

//
// Methods
//

void
Event::generateId (void)
{
        unsigned char uuid[50];
        char uuid_unparsed[37];

        // Generate a UUID based on current time. This method
        // hopefully provides UUIDs that can be ordered if necessary
        // (e.g. if two events stored to database storage with the
        // same storage time).
        uuid_generate_time (uuid);
        uuid_unparse (uuid, uuid_unparsed);
        uuid_clear (uuid);

        this->id = uuid_unparsed;
}

void
Event::accept (Storage& s)
{
        s.store (this);
}

//
// Accessor methods
//

string
Event::getId (void) const
{
        return this->id;
}

void
Event::setDataId (const string& theDataId)
{
        this->dataId = theDataId;
}

string
Event::getDataId (void) const
{
        return this->dataId;
}

void
Event::setMessage (const string& msg)
{
        this->message = msg;
}

string
Event::getMessage (void) const
{
        return this->message;
}

#ifdef GOT_DT_ERROR_CODE
void
Event::setErrorCode (errorCode err)
{
        this->errorCode = err;
}

errorCode
Event::getErrorCode (void) const
{
        return this->errorCode;
}
#endif

void
Event::setDatastreamId (const string& dsId)
{
        this->datastreamId = dsId;
}

string
Event::getDatastreamId (void) const
{
        return this->datastreamId;
}

void
Event::setDatastreamName (const string& dsName)
{
        this->datastreamName = dsName;
}

string
Event::getDatastreamName (void) const
{
        return this->datastreamName;
}

void
Event::setHostname (const string& hName)
{
        this->hostname = hName;
}

string
Event::getHostname (void) const
{
        return this->hostname;
}

#ifdef GOT_DT_LOG_LEVEL
void
Event::setLevel (logLevel lvl)
{
        this->level = lvl;
}

logLevel
Event::getLevel (void) const
{
        return this->level;
}
#endif

void
Event::setPid (pid_t thePid)
{
        this->pid = thePid;
}

pid_t
Event::getPid (void) const
{
        return this->pid;
}

void
Event::setTime (struct timeval theTime)
{
        this->time = theTime;
}

struct timeval
Event::getTime (void) const
{
        return this->time;
}
