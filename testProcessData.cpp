/*
 * Test dt::Datastream::process(dt::Data&)
 */
#include "config.h"
#include <futil/WmlDbg.h>
std::ofstream DBGSTREAM;

#include "src/Datastream.h"

using namespace std;
using namespace dt;

int main (int argc, char** argv) {

        if (argc < 2) {
                cerr << "Usage: testProcessData file [filter1 filter2]\n";
                cerr << "  The file should contain the input data to be passed to the filter chain\n";
                cerr << "  Optionally, filters through which to pass the file may be specified\n";
                return -1;
        }

        DBGOPEN ("./testProcessData.dbg");

        Data d (argv[1]);
        d.setUser ("dsuser");
        d.setName ("mydata");
        d.setTargetDatastream ("1300");

        list<string> filters;
        if (argc > 2) {
                string filterDir ("/usr/lib/cups/filter/");
                int i(2);
                while (i < argc) {
                        filters.push_back (filterDir + argv[i]);
                        ++i;
                }
        }

        Datastream ds (d.getTargetDatastream());

        cout << "Datastream ID: " << ds.getId() << endl;
        cout << "Datastream name: " << ds.getName() << endl;

        ds.setFilters (filters);
        ds.process (d);

        DBGCLOSE();
        return 0;
}
