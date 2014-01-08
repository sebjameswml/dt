//
// request_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// This file to become IPP request handler.

#include "request_handler.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include "mime_types.hpp"
#include "reply.hpp"
#include "request.hpp"

using namespace std;

http::server3::request_handler::request_handler (const std::string& doc_root)
        : doc_root_(doc_root)
{
}

void
http::server3::request_handler::handle_request (const request& req, reply& rep)
{
        bool expecting_continue = false;
        // For IPP; something like /printers/myprintername
        cerr << "Request uri (path&file): " << req.uri << endl;
        cerr << "Request method: " << req.method << endl;
        cerr << "Request version: " << req.http_version_major << "." << req.http_version_minor << endl;
        cerr << "Request headers:\n";
        auto i = req.headers.begin();
        while (i != req.headers.end()) {
                cerr << "header " << i->name << " = " <<  i->value << endl;
                if (i->name == "Expect" && i->value == "100-continue") {
                        expecting_continue = true;
                }
                ++i;
        }

        // Decode url to path.
        std::string request_path;
        if (!this->url_decode(req.uri, request_path)) {
                rep = reply::stock_reply(reply::bad_request);
                return;
        }

        // Request path must be absolute and not contain "..".
        if (request_path.empty() || request_path[0] != '/'
            || request_path.find("..") != std::string::npos) {
                rep = reply::stock_reply(reply::bad_request);
                return;
        }

        if (expecting_continue) {

                // Fill out the reply to be sent to the client.
                rep.status = reply::continue_response;
                rep.headers.resize(2);
                rep.headers[0].name = "Content-Length";
                rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
                rep.headers[1].name = "Content-Type";
                rep.headers[1].value = "application/ipp";

        } else {

                // Fill out the reply to be sent to the client.
                rep.status = reply::not_found;
                char buf[512] = { 'a', 'b', 'c', 0x0};
                rep.content.append (buf, 3);
                //while (is.read(buf, sizeof(buf)).gcount() > 0) {
                //        rep.content.append(buf, is.gcount());
                //}
                rep.headers.resize(2);
                rep.headers[0].name = "Content-Length";
                rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
                rep.headers[1].name = "Content-Type";
                rep.headers[1].value = "application/ipp";
        }
}

bool
http::server3::request_handler::url_decode (const std::string& in, std::string& out)
{
        out.clear();
        out.reserve(in.size());
        for (std::size_t i = 0; i < in.size(); ++i) {
                if (in[i] == '%') {
                        if (i + 3 <= in.size()) {
                                int value = 0;
                                std::istringstream is(in.substr(i + 1, 2));
                                if (is >> std::hex >> value) {
                                        out += static_cast<char>(value);
                                        i += 2;
                                } else {
                                        return false;
                                }
                        } else {
                                return false;
                        }
                } else if (in[i] == '+') {
                        out += ' ';
                } else {
                        out += in[i];
                }
        }
        return true;
}
