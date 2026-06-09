#include "pci_discovery.h"
#include <sys/io.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

// Low-level register access
uint32_t get_bar0(uint8_t bus, uint8_t slot, uint8_t func) {
    uint32_t config_addr = 0x80000000 | (bus << 16) | (slot << 11) | (func << 8) | 0x10;
    outl(config_addr, 0xCF8);
    return inl(0xCFC) & ~0xF;
}

// Unified initialization function
/*
TSAN tracks every memory access to detect races, 
but it does not know how to handle your direct hardware mapping (/dev/mem) or 
custom memory regions created via mmap. It views these regions as "untrusted" or 
"unexpected," leading it to panic and shut down.
*/
__attribute__((no_sanitize("thread")))
void init_hft_hardware(void) {
    printf("Initializing PCI hardware...\n");

    // Ensure IOPL is elevated for outl/inl
    if (iopl(3) != 0) {
        perror("Failed to get IO privileges");
    }
    // Example: Read Vendor ID from PCI config space (offset 0x00)
    uint32_t vendor_id = inl(0xCFC); 
    printf("PCI Vendor ID: 0x%X\n", vendor_id);

    // Example: Map BAR0 for your NICs here
    // uint32_t bar0 = get_bar0(0x03, 0x00, 0x00);
    // ... mmap logic ...
}