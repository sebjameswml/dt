/*
 *  This program is part of DT - a data transport daemon which implements
 *  IPP, LPD and other data transport/printing protocols.
 *
 *  DT is Copyright William Matthew Ltd. 2014.
 *
 *  Authors: Seb James <sjames@wmltd.co.uk>
 *           Tamora James <tjames@wmltd.co.uk>
 *
 *  DT is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  DT is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with DT.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*
 * This is a sample of a server which makes a connection from a client
 * into a keep-alive connection. It uses a variable in the scope of
 * the main function to store a number which is linked to each
 * client_context (client_context is held as a member of the
 * http::response). This number is a counter. On the first request
 * from the client, a response is sent with Connection: Keep-alive. On
 * a second request from the client, a response is sent with
 * Connection: Close.
 *
 * This is controlled here with
 * http::response::set_keep_alive_timeout.
 *
 * Note also, that in this example, if the client request contains
 * Expect: 100-continue, then a 100-continue is sent prior to the
 * response (controlled with http::response::set_respond_to_continue(bool).
 */

#include <iostream>
#include <sstream>
#include <native/native.h>
//#include "config.h"
//#include <futil/WmlDbg.h> // Included as we have futil, made necessary by wmlppcommon/DateTime.
//#include <wmlppcommon/DateTime.h> // For DateTime only
#include "src/ipp/Encoding.h"

using namespace native::http;
using namespace std;
using namespace dt;

//ofstream DBGSTREAM;

int main() {
    http server;
    int port = 8080;

//    DBGOPEN ("dt.log");

//    DBG ("Test");

    // Tell the server that it should respond to a client's Expect: 100-continue headers.
    server.set_respond_to_continue (true);

    // key: response id (pointer address), value "stage counter"
    std::map<native::http::response*, int> msg_number;

    if(!server.listen("0.0.0.0", port, [=,&msg_number](request& req, response& res) {

        cout << "User-registered callback START." << endl;

        // Using the memory address of response to identify
        // the connection - for each response in a kept-alive
        // connection, the response object is re-used, hence
        // its memory address will not change.
        try {
            msg_number[&res]++;
        } catch (std::out_of_range& e) {
            stringstream ss;
            ss << "Failed to record a message number count for the response with id "
               << hex << &res << endl;
            throw runtime_error (ss.str());
        }

        cout << " msg_number: " << msg_number.at(&res)
             << " for response ID " << hex << &res << endl;

        string body(""); // The data which will be sent in the response
        bool haveData = false;
        if (req.get_header ("Content-Type") == "application/ipp") {

            ipp::Encoding ippmsg (req.get_body());

            // What's the message?
            cout << "IPP message details:\n";

            cout << ippmsg.debugOutput();

            // In this very simple test, we'll say "if the message has
            // data, send an ok response, if not, send the "standard
            // manually created" response.

            ipp::Encoding ippResponse;
            ippResponse.setVersionNumber (1, 1);
            ippResponse.setRequestId (1);

            cerr << "Do we have data?" << endl;
            if ((haveData = ippmsg.hasData()) == true) {

                cerr << "We got a doc!\n";

                ippResponse.setStatusCode (ipp::successful_ok);

                ipp::Attr a("attributes-charset");
                a.setValue ("utf-8");
                ippResponse.addAttribute(a);

                a.reset("attributes-natural-language");
                a.setValue ("en-us");
                ippResponse.addAttribute(a);

                a.reset("job-uri");
                a.setValue ("ipp://192.168.0.6:8080/jobs/1212");
                ippResponse.addAttribute(a);

                a.reset("job-id");
                a.setValue (1212);
                ippResponse.addAttribute(a);

                a.reset("job-state");
                a.setValue (ipp::pending);
                ippResponse.addAttribute(a);

                a.reset("job-state-reasons");
                a.setValue ("none");
                ippResponse.addAttribute(a);

                a.reset("status-message");
                a.setValue("successful-ok");
                ippResponse.addAttribute(a);

            } else {

                cerr << "We got just commands...\n";

                // See what commands were.
                // ippmsg.command()?
                // ippmsg.getAttribute()? etc.
                short oid =  ippmsg.getOpIdOrStatusCode();
                if (oid == ipp::print_job) {
                        cerr << "Print job!" << endl;
                        ipp::Attr a("attributes-charset");
                        a.setValue ("utf-8");
                        ippResponse.addAttribute(a);

                        a.reset("attributes-natural-language");
                        a.setValue ("en-us");
                        ippResponse.addAttribute(a);

                        a.reset("printer-is-accepting-jobs");
                        a.setValue (true);
                        ippResponse.addAttribute(a);

                        a.reset("printer-state");
                        a.setValue (ipp::printer_processing);
                        ippResponse.addAttribute(a);

                        a.reset("printer-state-message");
                        a.setValue ("");
                        ippResponse.addAttribute(a);

                        a.reset("printer-state-reasons");
                        a.setValue ("none");
                        ippResponse.addAttribute(a);

                        a.reset("document-format-supported");
                        a.addValue ("application/octet-stream");
                        a.addValue ("application/openofficeps");
                        a.addValue ("application/pdf");
                        a.addValue ("application/postscript");
                        a.addValue ("application/rss+xml");
                        a.addValue ("application/vnd.cups-banner");
                        a.addValue ("application/vnd.cups-command");
                        a.addValue ("application/vnd.cups-form");
                        a.addValue ("application/vnd.cups-pdf");
                        a.addValue ("application/vnd.cups-postscript");
                        a.addValue ("application/vnd.cups-ppd");
                        a.addValue ("application/vnd.cups-raster");
                        a.addValue ("application/vnd.cups-raw");
                        a.addValue ("application/vnd.hp-hpgl");
                        a.addValue ("application/vnd.wml-pdf");
                        a.addValue ("application/x-cshell");
                        a.addValue ("application/x-csource");
                        a.addValue ("application/x-perl");
                        a.addValue ("application/x-shell");
                        a.addValue ("image/gif");
                        a.addValue ("image/jpeg");
                        a.addValue ("image/png");
                        a.addValue ("image/tiff");
                        a.addValue ("image/x-alias");
                        a.addValue ("image/x-bitmap");
                        a.addValue ("image/x-icon");
                        a.addValue ("image/x-photocd");
                        a.addValue ("image/x-portable-anymap");
                        a.addValue ("image/x-portable-bitmap");
                        a.addValue ("image/x-portable-graymap");
                        a.addValue ("image/x-portable-pixmap");
                        a.addValue ("image/x-sgi-rgb");
                        a.addValue ("image/x-sun-raster");
                        a.addValue ("image/x-xbitmap");
                        a.addValue ("image/x-xpixmap");
                        a.addValue ("image/x-xwindowdump");
                        a.addValue ("text/css");
                        a.addValue ("text/html");
                        a.addValue ("text/plain");
                        ippResponse.addAttribute(a);

                        a.reset("copies-supported");
                        a.setValue (1); // lower bound
                        a.addValue (9999); // upper bound
                        ippResponse.addAttribute(a);

                } else if (oid == ipp::get_job_attributes) {
                        cerr << "Get job attributes command" << endl;
                        ipp::Attr a("attributes-charset");
                        a.setValue ("utf-8");
                        ippResponse.addAttribute(a);

                        a.reset("attributes-natural-language");
                        a.setValue ("en-us");
                        ippResponse.addAttribute(a);

                        a.reset("job-media-sheets-completed");
                        a.setValue (0);
                        ippResponse.addAttribute(a);

                        a.reset("job-state");
                        a.setValue (ipp::completed);
                        ippResponse.addAttribute(a);

                } else if (oid == ipp::get_printer_attributes) {
                        cerr << "Get printer attributes command" << endl;
                        ipp::Attr a("attributes-charset");
                        a.setValue ("utf-8");
                        ippResponse.addAttribute(a);

                        a.reset("attributes-natural-language");
                        a.setValue ("en-us");
                        ippResponse.addAttribute(a);

                        a.reset("printer-state");
                        a.setValue (ipp::printer_idle);
                        ippResponse.addAttribute(a);

                        a.reset("printer-state-message");
                        a.setValue ("");
                        ippResponse.addAttribute(a);

                        a.reset("printer-state-reasons");
                        a.setValue ("none");
                        ippResponse.addAttribute(a);

                } else {
                        cerr << "Unexpected command: " << oid << endl;
                        ipp::Attr a("attributes-charset");
                        a.setValue ("utf-8");
                        ippResponse.addAttribute(a);

                        a.reset("attributes-natural-language");
                        a.setValue ("en-us");
                        ippResponse.addAttribute(a);
                }

                ippResponse.setOperationId (1);

            }

            body = ippResponse.encode();
            res.set_status(200);

        } else {
            // Send back a fail response
            res.set_status(500);
            res.end("Error");
            cerr << "Wrong content type: Returning (error)" << endl;
            return;
        }

        res.set_header ("Content-Type", "application/ipp");
        res.set_header ("Server", "DT/0.1alpha");

/* node.native should do the Date for us.
        wml::DateTime dt(time(NULL));
        dt.setRFC1123Format();
        res.set_header ("Date", dt.getFormatted());
*/
        // The following should set the Connection response header.
        /*if (haveData) {
                // Disable keepalive here?
                cerr << "Disable keep-alive" << endl;
                res.set_keep_alive_timeout(0);
                msg_number[&res] = 0;

                } else*/ if (msg_number[&res] <= 10) {
            // On the first message of the client connection, enable keep-alive
            cerr << "Enable keep-alive" << endl;
            res.set_keep_alive_timeout(30);
        } else {
            // On later message, disable keep-alive and reset the msg_number counter.
            cerr << "Disable keep-alive" << endl;
            res.set_keep_alive_timeout(0);
            msg_number[&res] = 0;
        }

        // We'll echo the body back in the response.
        if (body.empty()) {
            body = "no body to echo back";
        }
        cout << " Setting body to '" << body << "'" << endl;
        res.end (body);

        cout << "User-registered callback END" << endl;

    })) {
        cerr << "Failed to run server" << endl;
        return 1; // Failed to run server.
    }

    cout << "Server running at http://0.0.0.0:" << port << "/" << endl;
    return native::run();
}
