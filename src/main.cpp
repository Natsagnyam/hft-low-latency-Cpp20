#include <iostream>
#include <thread>
#include "control.hpp"
#include "packet_pipeline.hpp"
#include "rt_scheduler.hpp"
#include "test_latency.hpp" // Ensure this path is correct
#include "test_latency.hpp"

using namespace hft::engine;

int main() {
    // 1. Validate latency baseline before starting critical threads
    std::cout << "--- Starting Latency Validation Harness ---" << std::endl;
    run_latency_test();
    std::cout << "--- Latency Validation Complete ---" << std::endl;

    // 2. Initialize Control Planes
    producer_ctrl p_ctrl;
    consumer_ctrl c_ctrl;

    std::cout << "HFT Engine Initialized on Physical Cores 0 & 1." << std::endl;

    // 3. Producer Thread (Pinned to Physical Core 0)
    std::jthread producer([&p_ctrl]() {
        pin_thread_to_core(0); 
        while (true) {
            std::this_thread::yield(); 
        }
    });

    // 4. Consumer Thread (Pinned to Physical Core 1)
    std::jthread consumer([&c_ctrl]() {
        pin_thread_to_core(1);
        while (true) {
            std::this_thread::yield();
        }
    });

    std::cout << "Engine running. Press Ctrl+C to terminate." << std::endl;
    return 0;
}