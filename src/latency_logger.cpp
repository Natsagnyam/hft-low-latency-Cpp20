#include "../include/latency_logger.hpp"
#include <atomic>

namespace hft::engine {
    // Cold path: Writing to a log doesn't need to be as fast as the hot path.
    // Use memory_order_relaxed for metrics that don't require strict ordering.
    void log_latency(std::atomic<uint64_t>& counter, uint64_t value) {
        counter.store(value, std::memory_order_relaxed);
    }
}


#include "../include/latency_logger.hpp"
#include <x86intrin.h> // For __rdtscp

namespace hft::engine {
    uint64_t calculate_latency(const network_packet_t& pkt) {
        unsigned int aux;
        uint64_t now = __rdtscp(&aux);
        return now - pkt.start_cycles;
    }
}