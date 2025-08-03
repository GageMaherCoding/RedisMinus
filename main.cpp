#include "server.h"
#include <iostream>


int main() {
    Server server(6379);
    if (server.start()) {
        server.run();
    }
    else {
        std::cerr << "Server failed to start.\n";
    }
    return 0;
}
