#pragma once
#include <x86intrin.h>
#include <cstdint>

namespace hft::engine {
    // Reads the CPU Time Stamp Counter (TSC).
    // Using rdtscp ensures serialization to prevent out-of-order execution 
    // from polluting the timing samples.
    inline uint64_t get_tsc() {
        unsigned int aux;
        return __rdtscp(&aux);
    }
}