/*
 * Test dt::Datastream
 */
#include "config.h"
#include <futil/WmlDbg.h>
std::ofstream DBGSTREAM;

#include "src/Datastream.h"

using namespace std;
using namespace dt;

int main() {

        DBGOPEN ("./testDatastream.dbg");

        Datastream ds;
        cout << "Datastream ID: " << ds.getId() << endl;
        cout << "Datastream name: " << ds.getName() << endl;
        cout << endl;

        Datastream ds2 ("1300");

        cout << "Datastream ID: " << ds2.getId() << endl;
        cout << "Datastream name: " << ds2.getName() << endl;

        ds2.setName ("kyo1300");

        list<string> filters;
        filters.push_back ("/usr/lib/cups/filter/wmlkeyval");
//       filters.push_back ("/usr/lib/cups/filter/wmlpassthru");
        filters.push_back ("/usr/lib/cups/filter/wmlpdf");

        ds2.setFilters (filters);

        ds2.write();

        DBGCLOSE();
        return 0;
}
