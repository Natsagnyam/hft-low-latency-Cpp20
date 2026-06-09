#include "../include/packet_pipeline.hpp"
#include "../include/ring_buffer.hpp"
#include "../include/control.hpp"
#include <atomic>

namespace hft::engine {

    // Assuming a buffer size of 65536 (2^16) 
    static constexpr size_t BUFFER_SIZE = 65536;
    
    using Buffer = hft::engine::ring_buffer<BUFFER_SIZE>;

    // Implementation of the function inside the namespace    
    void process_packet(producer_ctrl& p_ctrl, const void* /*data*/) {
        uint32_t current_tail = p_ctrl.tail.load(std::memory_order_relaxed);
        
        // Wrap the index using the bitwise mask (BUFFER_SIZE - 1)
        uint32_t index = current_tail & (BUFFER_SIZE - 1);

        // ... Perform your data write using 'index' ...
        
        // Increment and wrap the tail for the next write
        uint32_t next_tail = (current_tail + 1) & (BUFFER_SIZE - 1);
        
        // Commit update with release semantics
        p_ctrl.tail.store(next_tail, std::memory_order_release);
    }

}