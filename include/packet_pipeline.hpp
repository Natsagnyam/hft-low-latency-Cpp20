#pragma once
#include <span>
#include <cstdint>
#include "hft_types.hpp"

namespace hft::engine {

    // Using constexpr for compile-time safety and performance
    static constexpr uint32_t RING_SIZE = 65536;
    static constexpr uint32_t BUFFER_MASK = RING_SIZE - 1;

    void init_pipeline();

    // Use std::span for modern, safe memory access
    int enqueue_packet(uint32_t id, std::span<const uint8_t> data);
    int dequeue_packet(network_packet_t& pkt);

} // namespace hft::engine