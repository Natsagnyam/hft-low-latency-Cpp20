#ifndef PACKET_PIPELINE_H
#define PACKET_PIPELINE_H

#include "hft_types.h"

#define RING_SIZE 65536
// Your Natska_Rule: Use bitwise & instead of modulo % for ring buffer indexing
#define BUFFER_MASK (RING_SIZE - 1)

void init_pipeline(void);
int enqueue_packet(uint32_t id, const uint8_t * restrict data, uint32_t len);
int dequeue_packet(network_packet_t * restrict pkt);

#endif // PACKET_PIPELINE_H