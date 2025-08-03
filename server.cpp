#include "server.h"
#include <iostream>
#include <ws2tcpip.h>
#include "command_parser.h"
#include "database.h"

#pragma comment(lib, "ws2_32.lib")

Server::Server(int port) : port(port), serverSocket(INVALID_SOCKET) {}

Server::~Server() {
    cleanup();
}

bool Server::start() {
    return initialize();
}

void Server::run() {
    std::cout << "Server running on port " << port << std::endl;
    Database db;

    SOCKET clientSocket;
    sockaddr_in clientAddr{};
    int clientSize = sizeof(clientAddr);

    // Accept clients in a loop
    while ((clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize)) != INVALID_SOCKET) {
        std::cout << "Client connected!\n";
        handleClient(clientSocket, db);  // Pass db reference
        closesocket(clientSocket);
    }

    std::cerr << "Accept failed\n";
}

void Server::handleClient(SOCKET clientSocket, Database& db) {
    const int bufferSize = 1024;
    char buffer[bufferSize];
    std::string partialCommand;

    while (true) {
        ZeroMemory(buffer, bufferSize);
        int bytesReceived = recv(clientSocket, buffer, bufferSize - 1, 0);
        if (bytesReceived <= 0) break;

        partialCommand += std::string(buffer, bytesReceived);

        // Check if a full command has been received (ending with '\n')
        size_t newlinePos;
        while ((newlinePos = partialCommand.find('\n')) != std::string::npos) {
            std::string fullCommand = partialCommand.substr(0, newlinePos);
            partialCommand.erase(0, newlinePos + 1);  // Remove processed part

            // Trim carriage return (\r) if it exists
            if (!fullCommand.empty() && fullCommand.back() == '\r') {
                fullCommand.pop_back();
            }

            // Process the command
            std::string response = CommandParser::parseAndExecute(fullCommand, db);
            send(clientSocket, response.c_str(), static_cast<int>(response.size()), 0);
        }
    }

    std::cout << "Client disconnected.\n";
}

bool Server::initialize() {
    WSADATA wsaData;
    int wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaResult != 0) {
        std::cerr << "WSAStartup failed: " << wsaResult << std::endl;
        return false;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        cleanup();
        return false;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed\n";
        cleanup();
        return false;
    }

    return true;
}

void Server::cleanup() {
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
    }
    WSACleanup();
}
