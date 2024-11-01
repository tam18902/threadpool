#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <arpa/inet.h>    // For socket operations
#include <unistd.h>       // For close() and sleep()
#include <sys/socket.h>   // For socket functions
#include <chrono>
#include <thread>

class Client {
public:
    Client() : Client("127.0.0.1", 8080){}
    Client(const char *server_ip, int port) : server_ip(server_ip), port(port), client_socket(-1) {}

    ~Client() {
        if (client_socket != -1) {
            close(client_socket);
        }
    }

    // Initialize and connect the client to the server
    bool connectToServer();
    // Start sending and receiving messages
    void run();

    void receiveMessages();
    void sendMessages();
private:
    const char *server_ip;
    int port;
    int client_socket;
};

#endif // CLIENT_H