#include "client.h"

bool Client::connectToServer() {
    // Step 1: Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        std::cerr << "Socket creation failed." << std::endl;
        return false;
    }

    // Step 2: Define server address
    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip, &server_address.sin_addr) <= 0) {
        std::cerr << "Invalid address or address not supported." << std::endl;
        close(client_socket);
        return false;
    }

    // Step 3: Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Connection to server failed." << std::endl;
        close(client_socket);
        return false;
    }

    std::cout << "Connected to server at " << server_ip << ":" << port << std::endl;
    return true;
}

void Client::sendMessages() {
    std::string message;
    while (true) {
        // Get message from user
        std::cout << "Enter message to send: ";
        std::getline(std::cin, message);
        // message = "Hello - " + std::to_string(rand());
        // Send the message to the server
        send(client_socket, message.c_str(), message.length(), 0);
        std::cout << "Message sent to server: " << message << std::endl;

        // Optionally wait before sending the next message
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

void Client::receiveMessages() {
    char buffer[1024] = {0};
    while (true) {
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';  // Null-terminate the received data
            std::cout << "Received response from server: " << buffer << std::endl;
        } else {
            std::cerr << "Failed to receive response from server or server disconnected." << std::endl;
            break;
        }
    }
}

void Client::run() {
    // Start threads for sending and receiving messages
    std::thread sendThread(&Client::sendMessages, this);
    std::thread receiveThread(&Client::receiveMessages, this);

    // Wait for the threads to finish (they won't in this case, as they run indefinitely)
    sendThread.join();
    receiveThread.join();
}
