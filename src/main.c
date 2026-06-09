#define _DEFAULT_SOURCE 
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>  // <-- Add this for getpid()
#include <signal.h>
#include <stdbool.h>
#include <stdatomic.h>

#include "hft_types.h"        // Contains network_packet
#include "packet_pipeline.h"  // Needs hft_types
#include "rt_scheduler.h"
#include "pci_discovery.h"    // Now provides init_hft_hardware
#include "main.h"
#include "latency_logger.h" 

static atomic_bool keep_running = ATOMIC_VAR_INIT(true);

void handle_signal(int sig) {
    atomic_store(&keep_running, false);
}

/**
 * Producer: Responsible for high-speed NIC polling and ring buffer enqueuing.
 * Pinned to Core 1 to ensure immediate hardware response.
 */

void* producer_thread(void* arg) {
    pin_thread_to_core(PRODUCER_CORE);        
    while (atomic_load(&keep_running)) { 
        // The NIC polling logic goes directly here
        // No inner while(1) allowed
    }    
    printf("Thread PRODUCER_CORE shutting down...\n"); // This confirms the loop exited
    return NULL;    
}


/**
 * Consumer: Responsible for Banking Logic and order execution.
 * Pinned to Core 2 to ensure logic execution is isolated from I/O jitter.
 */
void* consumer_thread(void* arg) {
    pin_thread_to_core(CONSUMER_CORE);
    network_packet_t pkt;        
    while (atomic_load(&keep_running)) {        
        log_latency(100);       // For testing ONLY:
        if (dequeue_packet(&pkt) == 0) {
            uint64_t latency = calculate_latency(&pkt);
            log_latency(latency); // Ensure this is actually saving to a local buffer
        }
        usleep(1000); // Prevent 100% CPU usage during test  // For testing ONLY:
    }
    printf("Thread CONSUMER_CORE shutting down...\n"); // This confirms the loop exited
    return NULL;
}




int main() {
    // Register the signal handler
    signal(SIGINT, handle_signal);
    printf("Initializing HFT Banking Engine...\n");
    printf("Initializing HFT Banking Engine (PID: %d)...\n", getpid());
    // 1. Hardware Initialization
    init_hft_hardware();
    init_pipeline();
    
    
    // 2. Thread Orchestration
    pthread_t p_thread, c_thread;

    if (pthread_create(&p_thread, NULL, producer_thread, NULL) != 0) return 1;
    if (pthread_create(&c_thread, NULL, consumer_thread, NULL) != 0) return 1;

   
    // Wait for a signal to stop (e.g., Ctrl+C)
    // When finished:
    // Your processing loop   
    // 3. Main thread acts as watchdog
    // Use atomic_load here as well
    while (atomic_load(&keep_running)) {
        usleep(100000); // 100ms sleep is fine here; not in the critical path
    }

    // Now this will be reached after Ctrl+C
    printf("Shutdown signal received. Saving data...\n");
    dump_latency_to_file("latency_results.csv");

    // 4. Join threads AFTER the file is saved (or before, depending on your needs)
    pthread_join(p_thread, NULL);
    pthread_join(c_thread, NULL);

    printf("Shutdown complete.\n");
    return 0;
}