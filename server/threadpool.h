// threadpool.h
#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>
#include <cstring>
#include <arpa/inet.h>    // For socket operations (Linux/Unix-specific)
#include <unistd.h>       // For close()
#include <sys/socket.h>   // For socket functions

// Enumeration for job types
enum class JobType {
    SEND,
    RECEIVE
};

// Forward declaration of the Server class
class Server;

// Placeholder for client connection, job, and status handling
struct ClientJob {
    int client_socket;
    JobType job_type;   // Type of job (SEND, RECEIVE, BROADCAST)
    std::string message; // Message to send or broadcast
};

class ThreadPool {
public:
    ThreadPool(size_t num_threads, Server* server_instance) : server(server_instance), stop(false) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers.emplace_back(&ThreadPool::workerThread, this);
        }
    }

    ~ThreadPool() {
        stop = true;
        condition.notify_all();
        for (auto &thread : workers) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    void addJob(ClientJob job) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            jobQueue.push(std::move(job));
        }
        condition.notify_one();
    }

private:
    Server* server;
    std::vector<std::thread> workers;
    std::queue<ClientJob> jobQueue;
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic<bool> stop;

    void workerThread();

    void processJob(ClientJob &job);
};

#endif // THREADPOOL_H