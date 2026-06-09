#pragma once
#include <cstddef>
#include <cstdint>
#include <concepts>

namespace hft::engine {
    template <size_t N>
    concept PowerOfTwo = (N > 0) && ((N & (N - 1)) == 0);

    template <size_t N>
    requires PowerOfTwo<N>
    class RingBuffer {
        static constexpr uint32_t MASK = static_cast<uint32_t>(N - 1);
        
    public:
        // Compile-time index calculation
        static constexpr uint32_t get_index(uint32_t ptr) noexcept {
            return ptr & MASK;
        }
    };
}