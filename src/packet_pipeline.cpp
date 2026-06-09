#include "../include/packet_pipeline.hpp"
#include "../include/ring_buffer.hpp"
#include "../include/control.hpp"
#include <atomic>

namespace hft::engine {

    // Assuming a buffer size of 65536 (2^16) 
    static constexpr size_t BUFFER_SIZE = 65536;
    using Buffer = RingBuffer<BUFFER_SIZE>;

    // Implementation of the function inside the namespace
    void process_packet(producer_ctrl& p_ctrl, const void* /*data*/) {
        
        // 1. Load the current tail
        // memory_order_relaxed is sufficient here because we are only 
        // using this value to calculate an offset for the local write
        uint32_t current_tail = p_ctrl.tail.load(std::memory_order_relaxed);
        
        // 2. Calculate index using bitwise AND (no modulo!)
        [[maybe_unused]] uint32_t index = Buffer::get_index(current_tail);
        
        // 3. Perform your data write here using 'index'...
        
        // 4. Increment and store with release semantics.
        // This ensures the data write (step 3) is visible to the consumer 
        // thread before the tail is updated.
        p_ctrl.tail.store(current_tail + 1, std::memory_order_release);
    }
}