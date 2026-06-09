
/*
latency_logger.c
*/
#include <stdio.h>
#include <stdint.h>
#include <stdatomic.h>
#include <x86intrin.h>
#include "latency_logger.h"

#define LOG_SIZE 1000000
static uint64_t latency_log[LOG_SIZE];
static atomic_uint log_idx = 0; // Thread-safe index

void log_latency(uint64_t latency) {
    uint32_t idx = atomic_fetch_add(&log_idx, 1);
    if (idx < LOG_SIZE) {
        latency_log[idx] = latency;
    }
}
void dump_latency_to_file(const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) return;
    for (uint32_t i = 0; i < log_idx; i++) {
        fprintf(f, "%lu\n", latency_log[i]);
    }
    fflush(f);   // <--- CRITICAL: Force write to disk    
    fclose(f);   // <--- CRITICAL: Ensures file handle is closed
}


uint64_t calculate_latency(network_packet_t* pkt) {
    uint64_t now = __rdtscp(NULL); // Read current CPU cycle
    return now - pkt->start_cycles;
}