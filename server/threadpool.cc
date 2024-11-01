#include "threadpool.h"
#include "server.h"

ThreadPool::ThreadPool(size_t num_threads, long sec_timeout, Server* server_instance) : server(server_instance), stop(false) {
    timeout.tv_sec = sec_timeout;  // seconds
    timeout.tv_usec = 0; // microseconds
    for (size_t i = 0; i < num_threads; ++i) {
        workers.emplace_back(&ThreadPool::workerThread, this);
    }
}

ThreadPool::~ThreadPool() {
    stop = true;
    condition.notify_all();
    for (auto &thread : workers) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void ThreadPool::addJob(ClientJob job) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobQueue.push(std::move(job));
    }
    condition.notify_one();
}

void ThreadPool::processJob(ClientJob &job) {
    // Step 1: Set a receive timeout for the socket (e.g., 5 seconds)
    setsockopt(job.client_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    if (job.job_type == JobType::RECEIVE) {
        char buffer[1024] = {0};
        int bytes_received = recv(job.client_socket, buffer, sizeof(buffer) - 1, 0);

        // Step 2: Check if data was received or if a timeout occurred
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';  // Null-terminate the received data
            std::cout << "Received message from client [" << job.client_socket << "]: " << buffer << std::endl;

            // Requeue the job for further processing
            addJob({job.client_socket, JobType::RECEIVE, ""});
            server->broadcastMessage(job.client_socket, buffer);

        } else if (bytes_received == 0) {
            std::cout << "Client disconnected." << std::endl;
            server->removeClientSocket(job.client_socket);
            close(job.client_socket);

        } else if (errno == EWOULDBLOCK || errno == EAGAIN) {
            // Timeout: No data received within the specified time
            // Requeue the job to check for other jobs, and reattempt later
            std::cout << "Job time out from [ "<< job.client_socket <<" ]" << std::endl;
            addJob({job.client_socket, JobType::RECEIVE, ""});

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