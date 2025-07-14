#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <thread>
#include <chrono>

int main() {
    // Create UDP socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Failed to create socket\n";
        return 1;
    }

    // Define server address
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "server", &server_addr.sin_addr);  // docker-compose DNS name

    // Open message file
    std::ifstream infile("/workspace/messages.txt");
    if (!infile.is_open()) {
        std::cerr << "Failed to open messages.txt\n";
        return 1;
    }

    std::string line;
    while (std::getline(infile, line)) {
        sendto(sock, line.c_str(), line.size(), 0, (sockaddr*)&server_addr, sizeof(server_addr));
        std::cout << "Client sent: " << line << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Send STOP signal
    const char* stop_msg = "STOP";
    sendto(sock, stop_msg, strlen(stop_msg), 0, (sockaddr*)&server_addr, sizeof(server_addr));
    std::cout << "Client sent: STOP" << std::endl;

    close(sock);
    return 0;
}
