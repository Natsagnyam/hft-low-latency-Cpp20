#include <iostream>
#include <vector>
#include "ring_buffer.hpp"
#include "tsc_clock.hpp"

namespace hft::engine {
    void run_latency_test() {
        ring_buffer<1024> buffer;
        network_packet_t pkt;
        
        uint64_t start = get_tsc();
        if (buffer.push(pkt)) {
            network_packet_t received_pkt;
            if (buffer.pop(received_pkt)) {
                uint64_t end = get_tsc();
                std::cout << "Round-trip latency: " << (end - start) << " cycles." << std::endl;
            }
        }
    }
}