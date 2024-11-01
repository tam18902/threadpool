#ifndef SERVER_H
#define SERVER_H

#include <algorithm>
#include "threadpool.h" // Include your thread pool header

class Server {
public:
    Server(size_t num_threads);

    bool initialize();
    
    void run();
    
    void broadcastMessage(int from_client_socket, const std::string& message);

    void addClientSocket(int client_socket);

    void removeClientSocket(int client_socket);
private:
    ThreadPool pool;
    int server_fd; // Server file descriptor
    const int PORT = 8080;
    std::vector<int> clientSockets; // List of client sockets
    std::mutex clients_mutex; // Mutex for client management
};

#endif // SERVER_H
