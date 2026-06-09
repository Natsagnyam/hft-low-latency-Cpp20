#include <iostream>
#include <thread>
#include "../include/control.hpp"
#include "../include/packet_pipeline.hpp"
#include "../include/rt_scheduler.hpp"

using namespace hft::engine;

int main() {
    // 1. Initialize Control Planes
    // alignas(64) ensures isolation on individual cache lines
    producer_ctrl p_ctrl;
    consumer_ctrl c_ctrl;

    std::cout << "HFT Engine Initialized on Physical Cores 0 & 1." << std::endl;

    // 2. Producer Thread (Pinned to Physical Core 0)
    std::jthread producer([&p_ctrl]() {
        pin_thread_to_core(0); 
        while (true) {
            // Producer logic: p_ctrl.tail.store(..., std::memory_order_release);
            std::this_thread::yield(); 
        }
    });

    // 3. Consumer Thread (Pinned to Physical Core 1)
    std::jthread consumer([&c_ctrl]() {
        pin_thread_to_core(1);
        while (true) {
            // Consumer logic: c_ctrl.head.load(std::memory_order_acquire);
            std::this_thread::yield();
        }
    });

    // 4. Main thread remains free to monitor or handle signals
    std::cout << "Engine running. Press Ctrl+C to terminate." << std::endl;
    
    // std::jthread automatically joins on scope exit
    return 0;
}