// hft_types.h

#ifndef HFT_TYPES_H
#define HFT_TYPES_H
#include <stdint.h>
#include <stdint.h>
#include <stdalign.h> // Required for alignas
#include <stdatomic.h> // Required for _Atomic

#define PACKET_PAYLOAD_SIZE 128


/* * Rule: No Atomic Battle. 
 * Separate structures are ensured to be on different Cache Lines via padding.
 * 64-byte alignment matches the standard x86 cache line size.
 */

 /* * Natska_Rule++ Alignment Layout Geometry
 * Pure ISO C18 alignment specification: placing alignas(64) on the struct structure 
 * members forces the compiler to pad the entire structure definition up to 256 bytes.
 */

struct producer_ctrl {
    uint8_t pad1[124]; 
    volatile uint32_t tail;
    uint8_t pad2[124]; 
} __attribute__((aligned(64)));

struct consumer_ctrl {
    uint8_t pad1[124];    
    volatile uint32_t head;
    uint8_t pad2[124];
} __attribute__((aligned(64)));

/* 1. Define the Protocol Data Unit (PDU) 
      Domain-Specific definition.           */
struct hft_trade_data {
    double price;
    uint32_t quantity;
    uint32_t side; // 0 for Buy, 1 for Sell
};

/* 2. Define the Transport Wrapper 
        Internal Representation         */



typedef struct {
    uint64_t start_cycles; // Required for latency benchmarking
    uint32_t packet_id;
    uint32_t payload_len;
    
    /* Union allows zero-copy access to structured trade data */
    union {
        uint8_t raw[PACKET_PAYLOAD_SIZE];
        struct hft_trade_data trade;
    } payload;
} network_packet_t;



#endif // HFT_TYPES_H