/*
 * Test dt::ipp::Encoding::parse
 */

#include <iostream>
#include <sstream>
#include <fstream>

#include "src/ipp/Encoding.h"

using namespace std;
using namespace dt;

int main (int argc, char** argv) {

        string body("");

        // Open the message from file
        if (argc < 2) {
                cerr << "Usage: testParse <file>\n";
                cerr << "  The file should contain the IPP payload as sent in the POST request\n";
                return -1;
        }

        string filename(argv[1]);
        ifstream f;
        f.open (filename.c_str(), ios::in);
        if (!f.is_open()) {
                cerr << "Failed to open file\n";
                return -1;
        }

        string line("");
        while (getline (f, line)) {
                body += line;
        }
        f.close();

        ipp::Encoding ippmsg(body);

        cout << "IPP message details:\n";
        cout << ippmsg.debugOutput();;

        return 0;
}
