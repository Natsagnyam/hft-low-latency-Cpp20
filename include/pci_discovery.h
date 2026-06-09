#ifndef PCI_DISCOVERY_H
#define PCI_DISCOVERY_H

#include <stdint.h>

// Returns the physical address of BAR0 for a given PCI device
uint32_t get_bar0(uint8_t bus, uint8_t slot, uint8_t func);
void init_hft_hardware(void);

#endif