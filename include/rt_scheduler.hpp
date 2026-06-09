#pragma once // Modern alternative to #ifndef guards
#include <cstdint>
#include <atomic>
#include <x86intrin.h>

namespace hft::engine {

    static constexpr size_t MAX_TASKS = 8;

    static inline uint64_t get_cycles() noexcept {
        unsigned int aux;
        return __rdtscp(&aux);
    }

    struct task_t {
        void (*task_func)();
        uint32_t priority;
        bool enabled; // C++ bool is cleaner than uint8_t
    };

    /* * Rule: No Atomic Battle. 
     * Using alignas(64) ensures each structure/member starts on a 
     * fresh cache line, preventing False Sharing.
     */
    struct alignas(64) scheduler_ctrl {
        task_t task_table[MAX_TASKS];
        
        // C++20 standard atomic
        std::atomic<uint32_t> active_mask{0};
        
        // Padding is automatically handled by the compiler 
        // to maintain the alignas(64) boundary.
    };

    // API
    void pin_thread_to_core(int core_id);
    void run_scheduler();
    void init_scheduler();

} // namespace hft::engine