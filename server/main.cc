#include "server.h"

int main() {
    size_t num_threads = 2; // Example: 2 threads in pool
    long time_out = 10; // second waiting for timeout a job
    Server server(num_threads, time_out);
    
    if (!server.initialize()) {
        return -1;  // Initialization failed
    }
    
    server.run();  // Run the server
    return 0;
}
