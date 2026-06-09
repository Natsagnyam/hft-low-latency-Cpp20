#pragma once
#include <cstdint>
#include "hft_types.hpp"

namespace hft::engine {

    /**
     * @brief Logs latency metrics. 
     * Note: This should ideally be handled by a lock-free buffer
     * to keep the hot path clean.
     */
    void log_latency(uint64_t latency);

    /**
     * @brief Flushes the latency data to disk.
     * This is a 'cold path' function and may perform blocking I/O.
     */
    void dump_latency_to_file(const char* filename);

    /**
     * @brief Calculates packet latency using the packet timestamp.
     */
    uint64_t calculate_latency(const network_packet_t& pkt);

} // namespace hft::engine