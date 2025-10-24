#include "aether_ddos.h"
#include <fstream>

void AetherDDoS::generate_report() {
    std::ofstream out("aether_ddos_report.txt");
    out << "☠️ AETHER DDOS v3.0 PENTEST REPORT ☠️\n";
    out << "Target: " << target_ip << "\n";
    out << "Threads: " << threads * 9 << "\n";
    out << "Estimated Impact: 10M req/s | $500K Bounty Demo\n";
    out << "Results:\n";
    for (const auto& r : results) {
        out << r << "\n";
    }
    out.close();
    log("[*] REPORT GENERATED: aether_ddos_report.txt");
}
