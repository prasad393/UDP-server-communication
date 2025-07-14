#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unordered_set>
#include <string>
#include <sstream>
#include <algorithm>

#define PORT 8080

int main() {
    int sockfd;
    struct sockaddr_in servaddr{}, cliaddr{};
    socklen_t len = sizeof(cliaddr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        std::cerr << "Bind failed\n";
        return 1;
    }

    std::cout << "Server: Listening for packets on port " << PORT << "...\n";

    std::unordered_set<int> received_seq;
    int last_seq = -1;

    while (true) {
        char buffer[1024] = {0};  // buffer for SEQ|DATA messages
        ssize_t n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                             (struct sockaddr*)&cliaddr, &len);
        if (n <= 0) break;

        buffer[n] = '\0';  // Null-terminate

        // Handle "STOP"
        if (strncmp(buffer, "STOP", 4) == 0) {
            std::cout << "Received STOP. Terminating server.\n";
            break;
        }

        std::string msg(buffer);
        std::istringstream iss(msg);
        std::string seq_str, data_str;

        if (!getline(iss, seq_str, '|') || !getline(iss, data_str)) {
            std::cerr << "Invalid message format: " << msg << "\n";
            continue;
        }

        int seq_num = std::stoi(seq_str);
        std::string status;

        if (received_seq.find(seq_num) != received_seq.end()) {
            status = "Duplicate";
        } else {
            if (seq_num == last_seq + 1) {
                status = "In Order";
            } else if (seq_num > last_seq + 1) {
                int missing_start = last_seq + 1;
                int missing_end = seq_num - 1;
                if (missing_start == missing_end) {
                    status = "Packet Loss (Missing SEQ: " + std::to_string(missing_start) + ")";
                } else {
                    status = "Packet Loss (Missing SEQ: " +
                             std::to_string(missing_start) + " to " + std::to_string(missing_end) + ")";
                }
            } else {
                status = "Out of Order";
            }

            received_seq.insert(seq_num);
            last_seq = std::max(last_seq, seq_num);
        }

        std::cout << "Received SEQ: " << seq_num
                  << " | DATA: " << data_str
                  << " | Status: " << status << "\n";
    }

    close(sockfd);
    return 0;
}
