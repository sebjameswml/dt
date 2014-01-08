/*
 * Test dt::ipp::Encoding::encode
 */

#include <iostream>
#include <sstream>

#include "src/ipp/Encoding.h"

using namespace std;
using namespace dt;

void debugout (const string& encoded)
{
        cout << "Encoded string:" << endl;
        for (unsigned int i = 0; i<encoded.size(); ++i) {
                char c = encoded[i];
                if (c == 0x9 || c == 0xa || c == 0xd || (c >= 0x20 && c <=0x7e)) {
                        // was printable
                        cout << c;
                        cout << "..";
                } else {
                        // Unprintable.
                        cout << "#..";
                }
        }
        cout << endl;
        for (unsigned int i = 0; i<encoded.size(); ++i) {
                cout.width(2);
                cout.fill('0');
                cout << hex << (static_cast<int>(encoded[i]) & 0xff) << ".";
        }
        cout << endl;
}

int main() {

        ipp::Encoding ippResponse;
        ippResponse.setVersionNumber (1, 1);
        ippResponse.setOperationId (1);
        ippResponse.setRequestId (1);

        ipp::Attr a("attributes-charset");
        a.setValue ("utf-8");

        ipp::Attr b("attributes-natural-language");
        b.setValue ("en-us");

        ipp::Attr c("document-format-supported");
        c.addValue ("application/octet-stream");
        c.addValue ("application/pdf");
        c.addValue ("application/postscript");

        // Choose which to add to ippResponse:
        //ippResponse.addAttribute(c);

//        a.reset("printer-state");
//        a.setValue (13);
//        ippResponse.addAttribute(a);

//        a.reset("printer-state-message");
//        a.setValue ("");
//        ippResponse.addAttribute(a);

        a.reset ("job-id");
        a.setValue (1212);
        ippResponse.addAttribute(a);

        string encoded = ippResponse.encode();

        // Either:
        //cout << encoded;#
        // Or:
        debugout (encoded);

        return 0;
}
