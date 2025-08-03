#pragma once
#include <winsock2.h>
#include <string>
#include "database.h"

class Server {
public:
    Server(int port);
    ~Server();
    bool start();
    void run();

private:
    int port;
    SOCKET serverSocket;
    bool initialize();
    void cleanup();
    void handleClient(SOCKET clientSocket, Database& db);

};
