/*
 * Test dt::Datastream::process(dt::Data&)
 */
#include "config.h"
#include <futil/WmlDbg.h>
std::ofstream DBGSTREAM;

#include "src/Datastream.h"

using namespace std;
using namespace dt;

int main() {

        DBGOPEN ("./testProcessData.dbg");

        Data d ("test_df.xml");
        d.setUser ("dsuser");
        d.setName ("mydata");
        d.setTargetDatastream ("1300");

        list<string> filters;
        filters.push_back ("/usr/lib/cups/filter/wmlkeyval");
//       filters.push_back ("/usr/lib/cups/filter/wmlpassthru");
        filters.push_back ("/usr/lib/cups/filter/wmlpdf");

        Datastream ds (d.getTargetDatastream());

        cout << "Datastream ID: " << ds.getId() << endl;
        cout << "Datastream name: " << ds.getName() << endl;

        ds.setFilters (filters);
        ds.process (d);

        DBGCLOSE();
        return 0;
}
