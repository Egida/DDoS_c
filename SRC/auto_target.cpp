#include "aether_ddos.h"
#include <sys/socket.h>
#include <netinet/in.h>

std::vector<int> AetherDDoS::auto_target() {
    std::vector<int> open_ports;
    std::vector<int> common_ports = {80, 443, 8080, 22, 3306};

    log("[*] AUTO-TARGET SCANNING...");
    for (int port : common_ports) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, target_ip.c_str(), &addr.sin_addr);

        if (connect(sock, (sockaddr*)&addr, sizeof(addr)) >= 0) {
            open_ports.push_back(port);
            log("[*] OPEN PORT: " + std::to_string(port));
        }
        close(sock);
    }
    return open_ports;
}
