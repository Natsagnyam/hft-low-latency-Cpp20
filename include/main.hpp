#pragma once
#include <cstdint>

namespace hft::engine::config {

    // Using constexpr instead of #define for type safety
    constexpr int WATCHDOG_CORE = 0;
    
    // Physical core 0: Threads 0 and 2
    constexpr int PRODUCER_CORE = 0;
    
    // Physical core 1: Threads 1 and 3
    constexpr int CONSUMER_CORE = 1; 

    // Recommendation: Avoid using threads 0 and 2 simultaneously if possible,
    // as they fight for the same physical core execution units.
}