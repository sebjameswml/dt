#ifdef __GNUG__
#  pragma implementation
#endif

#include "config.h"

#include "Event.h"
#include "SyslogStorage.h"

using namespace std;
using namespace dt;
using namespace dt::log;

// Constructors/Destructors
//

SyslogStorage::SyslogStorage (int facility)
{
        /* Set up syslogger */
        openlog ("dt::log", LOG_PID, facility);

        syslog (LOG_INFO, "%s: dt::log::SyslogStorage active.", __FUNCTION__);
}

SyslogStorage::~SyslogStorage()
{
}

//
// Methods
//

void
SyslogStorage::store (Event* e)
{
#ifdef GOT_DT_LOG_LEVEL
        int level (e.getLogLevel());
#else
        int level (LOG_NOTICE);
#endif
        syslog (level,
                "Event %s for data/job %s on datastream %s [%s]: %s",
                e->getId().c_str(),
                e->getJobId().c_str(),
                e->getDatastreamName().c_str(),
                e->getDatastreamId().c_str(),
                e->getMessage().c_str());

        // System logger already includes these:
        // e->getHostname() // Hostname
        // e->getPid() // Process ID
}

// Accessor methods
//


// Other methods
//


