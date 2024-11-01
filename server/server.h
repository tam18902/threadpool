#ifndef SERVER_H
#define SERVER_H

#include "threadpool.h"

class Server {
public:
    Server(size_t num_threads, long timeout);

    bool initialize();
    
    void run();
    
    void broadcastMessage(int from_client_socket, const std::string& message);

    void addClientSocket(int client_socket);

    void removeClientSocket(int client_socket);
private:
    const int PORT = 8080;
    int server_fd; // Server file descriptor
    
    // ThreadPool where to add job for workings
    ThreadPool pool;

    // List of client sockets - Mutex for client management
    std::vector<int> clientSockets; 
    std::mutex clients_mutex; 
};

#endif // SERVER_H
