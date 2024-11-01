#include "threadpool.h"
#include "server.h"

void ThreadPool::processJob(ClientJob &job) {
    if (job.job_type == JobType::RECEIVE) {
        char buffer[1024] = {0};
        int bytes_received = recv(job.client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';  // Null-terminate the received data
            std::cout << "Received message from client [" << job.client_socket << "]: " << buffer << std::endl;

            // Optionally send a response back to the client
            // const char *response = "Message received";
            // send(job.client_socket, response, strlen(response), 0);

            // Requeue the job for further processing if needed
            addJob({job.client_socket, JobType::RECEIVE, ""});
            server->broadcastMessage(job.client_socket, buffer);
        } else if (bytes_received == 0) {
            std::cout << "Client disconnected." << std::endl;
            server->removeClientSocket(job.client_socket);
            close(job.client_socket);
        } else {
            std::cerr << "Error receiving message from client." << std::endl;
            close(job.client_socket);
        }
    } else if (job.job_type == JobType::SEND) {
        send(job.client_socket, job.message.c_str(), job.message.length(), 0);
        std::cout << "Sent message to client [" << job.client_socket << "]: " << job.message << std::endl;
    }
}


void ThreadPool::workerThread() {
    while (true) {
        ClientJob job;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            condition.wait(lock, [this] { return stop || !jobQueue.empty(); });
            if (stop && jobQueue.empty()) return;

            job = std::move(jobQueue.front());
            jobQueue.pop();
        }
        processJob(job);
    }
}