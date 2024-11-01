#include "server.h"

Server::Server(size_t num_threads) : pool(num_threads, this), server_fd(-1) {}

bool Server::initialize() {
    // Create a TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "Socket creation failed." << std::endl;
        return false;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed." << std::endl;
        close(server_fd);
        return false;
    }

    // Listen for incoming connections
    if (listen(server_fd, 10) < 0) {
        std::cerr << "Listen failed." << std::endl;
        close(server_fd);
        return false;
    }

    std::cout << "Server listening on port " << PORT << std::endl;
    return true;
}

void Server::run() {
    while (true) {
        socklen_t addrlen = sizeof(sockaddr_in);
        int client_socket = accept(server_fd, nullptr, &addrlen);
        if (client_socket < 0) {
            std::cerr << "Failed to accept connection." << std::endl;
            continue;  // Continue to the next loop iteration
        }

        std::cout << "Accepted connection from client." << std::endl;
        addClientSocket(client_socket);

        // Add job for receiving messages
        pool.addJob({client_socket, JobType::RECEIVE, ""});
    }

    close(server_fd);
}

void Server::broadcastMessage(int from_client_socket, const std::string& message) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    // Broadcast the message to all other clients
    for (int socket : clientSockets) {
        if (socket != from_client_socket) { // Avoid sending back to sender
            pool.addJob({socket, JobType::SEND, message});
        }
    }
}

void Server::addClientSocket(int client_socket) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    clientSockets.push_back(client_socket);
}

void Server::removeClientSocket(int client_socket) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), client_socket), clientSockets.end());
}