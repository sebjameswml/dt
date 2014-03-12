/*
 * Test dt::log::Event and dg::log::Storage
 */
#include <syslog.h>

#include "src/log/Event.h"
#include "src/log/SyslogStorage.h"
#include "src/log/MySQLStorage.h"

using namespace std;
using namespace dt::log;

int main() {

        Event e;
        e.setDataId("12345");
        e.setDatastreamName("input1");
        e.setDatastreamId("921");
        e.setMessage ("An event occurred");

        SyslogStorage s;

        e.accept (s);


        Event e2;
        e2.setDataId("12346");
        e2.setDatastreamName("input2");
        e2.setDatastreamId("922");
        e2.setMessage ("Another event occurred");

        MySQLStorage s2;
        e2.accept (s2);

        return 0;
}
