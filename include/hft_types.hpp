#pragma once
#include <cstdint>
#include <atomic>
#include <new> 

namespace hft::engine {

    static constexpr size_t PACKET_PAYLOAD_SIZE = 128;

    /* * Rule: No Atomic Battle.
     * alignas(64) ensures each structure occupies its own cache line, 
     * preventing False Sharing.
     */

    struct alignas(64) producer_ctrl {
        std::atomic<uint32_t> tail{0};
    };

    struct alignas(64) consumer_ctrl {
        std::atomic<uint32_t> head{0};
    };

    struct hft_trade_data {
        double price;
        uint32_t quantity;
        uint32_t side; // 0 for Buy, 1 for Sell
    };

    struct network_packet_t {
        uint64_t start_cycles;
        uint32_t packet_id;
        uint32_t payload_len;

        // Union for zero-copy access
        union {
            uint8_t raw[PACKET_PAYLOAD_SIZE];
            hft_trade_data trade;
        } payload;
    };

} // namespace hft::engine