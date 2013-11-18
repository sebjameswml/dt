#include <iostream>
#include <native/native.h>
using namespace native::http;

int main() {
    http server;
    if(!server.listen("0.0.0.0", 8080, [](request& req, response& res) {
        res.set_status(200);
        res.set_header("Content-Type", "text/plain");
        res.end("node.native server\n");
    })) return 1; // Failed to run server.

    std::cout << "Server running at http://0.0.0.0:8080/" << std::endl;
    return native::run();
}
