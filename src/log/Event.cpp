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
        , jobId("")
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


// Accessor methods
//
