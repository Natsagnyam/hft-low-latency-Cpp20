#ifndef RT_SCHEDULER_H
#define RT_SCHEDULER_H

#include <stdint.h>
#include <stdatomic.h>
#include <stdalign.h>
#include <x86intrin.h> // REQUIRED for __rdtscp

#define MAX_TASKS 8


static inline uint64_t get_cycles(void) {
    unsigned int aux;
    return __rdtscp(&aux);
}

typedef struct {
    void (*task_func)(void);
    uint32_t priority;
    uint8_t enabled;
} task_t;

/* * Natska_Rule++ Control Plane:
 * Padded to 256 bytes (4x 64-byte lines) to guarantee the 
 * active_mask is isolated from hardware cache line invalidation.
 */
struct scheduler_ctrl {
    alignas(64) uint8_t pad1[124];
    task_t task_table[MAX_TASKS];
    _Atomic uint32_t active_mask;  // Data Race sorted by _Atomic. It happens as a single, uninterruptible transaction
    // _Atomic forces the CPU to synchronize that memory across cores
    uint8_t pad2[124];
};

/*
// To set a bit:
atomic_fetch_or(&sched.active_mask, (1 << i));

// To clear a bit:
atomic_fetch_and(&sched.active_mask, ~(1 << i));

// To check a bit:
if (atomic_load(&sched.active_mask) & (1 << i)) { ... }
*/

// API
void pin_thread_to_core(int core_id);
void run_scheduler(void);
void init_scheduler(void);

#endif // RT_SCHEDULER_H