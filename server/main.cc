#include "server.h"

int main() {
    size_t num_threads = 4; // Example: 4 threads in pool
    Server server(num_threads);
    
    if (!server.initialize()) {
        return -1;  // Initialization failed
    }
    
    server.run();  // Run the server
    return 0;
}
