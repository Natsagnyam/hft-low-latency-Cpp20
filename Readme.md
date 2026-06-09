# HFT Banking Engine: Ultra-Low Latency Trading Infrastructure

This project implements a high-performance, deterministic trading engine designed for US Equities and Options. The architecture prioritizes **jitter-free execution** and **single-digit microsecond latency** by bypassing standard kernel networking stacks.

## Core Architectural Principles

* **Kernel Bypass & Driverless DMA**: Communicates directly with NIC hardware via PCI BAR0 and Huge Pages, eliminating context-switching overhead.
* **Zero-Jitter Execution**:
* **No Atomic Battle**: Separate structures are ensured to be on different cache lines via padding.
* **Deterministic Ring Buffers**: Uses bitwise `&` operations for buffer indexing instead of modulo `%`.
* **Memory Management**: Strictly avoids `malloc` during runtime, favoring static allocation.


* **Hardware Alignment**:
* Structures are aligned to 64-byte boundaries (Cache Lines).
* CPU pinning and core isolation are utilized to minimize OS-induced latency.



## Technical Stack

* **Language Standard**: Built with C18, transitioning to C++20 for high-performance components.
* **System Environment**: Optimized for Linux, utilizing memory-mapped I/O and `taskset` for process affinity.
* **Concurrency**: Features lock-free producer/consumer structures designed to prevent contention between critical trading threads.

## Build and Run

### Prerequisites

* Compiler: `gcc` with `-march=native` support.
* Environment: `hugepages` configured on the target Linux system.

### Compilation

The project utilizes a modular `Makefile`. To build with ThreadSanitizer for safety validation:

```bash
make clean
make debug-tsan

```

### Execution

The engine requires elevated privileges to access PCI registers and configure CPU affinity:

```bash
sudo taskset -c 0,1,2 setarch $(uname -m) -R ./hft_engine

```

## Performance Benchmarking

The engine includes a `latency_logger` that persists cycle-accurate timing data to a CSV file. The included `analyze_latency.py` script provides statistical analysis and histogram generation for P99.9 latency monitoring.

---

*Developed by Natsagnyam Namkhai (Natska)*
*Principal Ultra-Low Latency Infrastructure & Systems Engineer*



### Telemetry & Latency Logging
### How the Log File Works
The logging system captures the difference between the CPU cycle count at packet arrival (producer_thread) and packet execution (consumer_thread).

Generation: It uses __rdtscp to capture cycle-accurate timestamps.

Storage: Results are written to a static, pre-allocated latency_log array to avoid runtime memory allocation (malloc).

Visualization: The analyze_latency.py script parses this CSV to draw a histogram, identifying "tail latency" (P99/P99.9), which is crucial for identifying jitter in ULL systems.

### Does Logging hurt ULL?
In this implementation: No. We have architected the logger to avoid "hurting" the hot path through these techniques:

No System Calls on Hot Path: The log_latency function is a simple atomic index increment and array write. It does not perform file I/O during execution.

Asynchronous I/O: The dump_latency_to_file function is only triggered during the shutdown sequence (via SIGINT). This ensures the I/O bottleneck (opening/writing files) never interferes with packet processing.

Static Allocation: We use static arrays instead of heap allocation, preventing memory fragmentation and malloc latency spikes.

Cache Isolation: The logger operates on its own memory space, and by utilizing atomic_uint for the index, we ensure there is no contention between the producer and consumer threads.

Logging is essentially "free" during the runtime of the engine. The engine remains pure ULL during operation, with all heavy processing delayed until the shutdown phase.

### Understanding the Trade-off
To help you understand the architecture, consider this:

By separating the hot path (where packets flow) from the control path (where logging occurs), you have effectively bypassed the usual ULL performance penalties.

Summary for your README:

Hot Path: 0 syscalls, 0 allocations, 0 blocking.

Control Path: Only active during process exit.

This README ensures that any contributor (or yourself in the future) understands why the logger is safe to use in a production ULL environment.