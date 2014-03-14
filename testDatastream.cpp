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

        Datastream ds2 ("12345");

        cout << "Datastream ID: " << ds2.getId() << endl;
        cout << "Datastream name: " << ds2.getName() << endl;

        DBGCLOSE();
        return 0;
}
