#include <sys/io.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "../include/pci_discovery.hpp"

namespace hft::engine {

    // Low-level register access using x86 port I/O
    uint32_t get_bar0(uint8_t bus, uint8_t slot, uint8_t func) {
        // Construct configuration space address (PCI Express/PCI Mechanism 1)
        uint32_t config_addr = 0x80000000 | (bus << 16) | (slot << 11) | (func << 8) | 0x10;
        outl(config_addr, 0xCF8);
        return inl(0xCFC) & ~0xF;
    }

    // Hardware init with TSAN exemption for raw memory/port access
    __attribute__((no_sanitize("thread")))
    void init_hft_hardware() {
        printf("Initializing PCI hardware...\n");

        // Elevate IO privileges to permit port I/O
        if (iopl(3) != 0) {
            perror("Failed to get IO privileges. Are you running as root?");
            return;
        }

        // Example: Probe PCI config space
        uint32_t vendor_id = inl(0xCFC); 
        printf("PCI Vendor ID: 0x%X\n", vendor_id);
    }
}