#ifndef LATENCY_LOGGER_H
#define LATENCY_LOGGER_H

#include <stdint.h>
#include "hft_types.h" 

void log_latency(uint64_t latency);
void dump_latency_to_file(const char* filename);
// Add this line:
uint64_t calculate_latency(network_packet_t* pkt); 

#endif
