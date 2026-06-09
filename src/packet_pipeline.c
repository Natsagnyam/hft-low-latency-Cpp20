#define _POSIX_C_SOURCE 199309L
#define _GNU_SOURCE
#include <x86intrin.h> // Required for __rdtscp
#include <stdio.h>
#include "packet_pipeline.h"
#include <time.h>
#include <string.h>
#include <assert.h>
#include <stdatomic.h>
#include <stdalign.h>
#include "latency_logger.h" 

// Cycle counter helper
static inline uint64_t get_cycles(void) {
    unsigned int aux;
    return __rdtscp(&aux);
}

// Align ring buffer to cache line
alignas(64) static network_packet_t ring_buffer[RING_SIZE];
alignas(64) static struct producer_ctrl g_producer;
alignas(64) static struct consumer_ctrl g_consumer;

static_assert(sizeof(struct producer_ctrl) == 256, "CRITICAL: Producer layout geometry broken!");
static_assert(sizeof(struct consumer_ctrl) == 256, "CRITICAL: Consumer layout geometry broken!");

void init_pipeline(void) {
    atomic_init(&g_producer.tail, 0);
    atomic_init(&g_consumer.head, 0);
    memset(ring_buffer, 0, sizeof(ring_buffer));
}

int enqueue_packet(uint32_t id, const uint8_t *data, uint32_t len) {
    uint32_t current_tail = atomic_load_explicit(&g_producer.tail, memory_order_relaxed);
    uint32_t current_head = atomic_load_explicit(&g_consumer.head, memory_order_acquire);

    if ((current_tail - current_head) >= RING_SIZE) {
        return -1; 
    }

    uint32_t index = current_tail & BUFFER_MASK;
    
    // Record start time in the packet metadata
    ring_buffer[index].start_cycles = get_cycles();
    ring_buffer[index].packet_id = id;
    ring_buffer[index].payload_len = (len > PACKET_PAYLOAD_SIZE) ? PACKET_PAYLOAD_SIZE : len;
    
    if (data && len > 0) {
        memcpy(ring_buffer[index].payload.raw, data, ring_buffer[index].payload_len);
    }
  
    atomic_store_explicit(&g_producer.tail, current_tail + 1, memory_order_release);
    return 0;
}

int dequeue_packet(network_packet_t *out_packet) {
    uint32_t current_tail = atomic_load_explicit(&g_producer.tail, memory_order_acquire);
    uint32_t current_head = atomic_load_explicit(&g_consumer.head, memory_order_relaxed);

    if (current_head == current_tail) {
        return -1; 
    }

    uint32_t index = current_head & BUFFER_MASK;
    *out_packet = ring_buffer[index];

    // FIX: Calculate latency using the pointer 'out_packet'
    uint64_t end_time = get_cycles();
    uint64_t latency = end_time - out_packet->start_cycles;
    
    // FIX: Log using the logger function
    log_latency(latency);

    // FIX: Access the trade data via the pointer
    double price = out_packet->payload.trade.price; 
    (void)price; // Suppress unused warning for now

    atomic_store_explicit(&g_consumer.head, current_head + 1, memory_order_release);
    return 0;
}