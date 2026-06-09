#pragma once
#include <cstdint>

namespace hft::engine {

    /**
     * @brief Returns the physical address of BAR0 for a given PCI device.
     * PCI discovery is part of the 'cold path' initialization, 
     * where standard C++ types are preferred for safety.
     */
    uint32_t get_bar0(uint8_t bus, uint8_t slot, uint8_t func);

    /**
     * @brief Initializes HFT-specific hardware registers.
     */
    void init_hft_hardware();

} // namespace hft::engine