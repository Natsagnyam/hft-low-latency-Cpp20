#pragma once
#include "hft_types.hpp"
#include <array>

namespace hft::engine {
    template<size_t N>
    struct ring_buffer {
        static_assert((N & (N - 1)) == 0, "Ring buffer size must be a power of 2");

        std::array<network_packet_t, N> data;
        producer_ctrl p_ctrl;
        consumer_ctrl c_ctrl;

        bool push(const network_packet_t& packet) {
            uint32_t t = p_ctrl.tail.load(std::memory_order_relaxed);
            uint32_t next_t = (t + 1) & (N - 1);
            if (next_t == c_ctrl.head.load(std::memory_order_acquire)) return false;
            data[t] = packet;
            p_ctrl.tail.store(next_t, std::memory_order_release);
            return true;
        }

        bool pop(network_packet_t& packet) {
            uint32_t h = c_ctrl.head.load(std::memory_order_relaxed);
            if (h == p_ctrl.tail.load(std::memory_order_acquire)) return false;
            packet = data[h];
            c_ctrl.head.store((h + 1) & (N - 1), std::memory_order_release);
            return true;
        }
    };
}