#include "client.h"

int main() {
    Client client;
    if (client.connectToServer()) {
        client.run();
    }
    return 0;
}
