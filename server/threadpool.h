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
    JobType job_type;   // Type of job (SEND, RECEIVE)
    std::string message;    // String store message for sending or received message
};

class ThreadPool {
public:
    ThreadPool(size_t num_threads, long sec_timeout, Server* server_instance);
    ~ThreadPool();

    // Public method for enqueue job
    void addJob(ClientJob job);

private:
    // server instance for call back to send broadcast message
    Server* server;

    // Handle worker thread
    std::vector<std::thread> workers;
    
    // Queue job need to be handle
    std::queue<ClientJob> jobQueue;
    std::mutex queue_mutex;
    
    // Struct set timeout for socket
    struct timeval timeout;

    // Condition and stop - bool value for waiting and notify 
    // for worker know when need to wake or stop
    std::condition_variable condition;
    std::atomic<bool> stop;

    // What worker do
    void workerThread();

    // When job really handle
    void processJob(ClientJob &job);
};

#endif // THREADPOOL_H