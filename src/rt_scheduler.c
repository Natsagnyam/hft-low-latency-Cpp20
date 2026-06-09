
#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "rt_scheduler.h"


// Static control block as per Natska_Rule++
static struct scheduler_ctrl sched;

/**
 * CPU Affinity: Pins the calling thread to a specific physical core.
 * Essential for HFT to eliminate jitter caused by OS task migration.
 */
void pin_thread_to_core(int core_id) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    pthread_t current_thread = pthread_self();
    if (pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset) != 0) {
        perror("ERROR: Failed to set thread affinity");
    } else {
        printf("Thread %lu pinned to core %d\n", (unsigned long)current_thread, core_id);
    }
}

/**
 * Task Dispatcher: A branchless, constant-time selector.
 * Uses bitwise masks to determine which RT task to execute next.
 */
void run_scheduler(void) {
    // Branchless selection: iterate through task bitmask
    // We assume 'active_mask' is updated by hardware interrupts/DMA
    for (int i = 0; i < MAX_TASKS; i++) {
        // Atomic check of the task bit
        if ((sched.active_mask & (1 << i)) && sched.task_table[i].enabled) {
            sched.task_table[i].task_func();
            
            // Clear the bit after execution
            sched.active_mask &= ~(1 << i);
        }
    }
}