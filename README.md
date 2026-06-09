# HFT Banking Engine (C++20)

A high-performance, low-latency trading engine designed for deterministic execution on Linux. This engine utilizes strict hardware-level isolation, lock-free synchronization, and physical core affinity to achieve microsecond-level performance.

## Architecture
- **Language:** C++20
- **Synchronization:** Lock-free, utilizing `std::atomic` with explicit memory ordering (Acquire/Release) to eliminate atomic contention ("Atomic Battle").
- **Hardware Isolation:** - Cache-line alignment (`alignas(64)`) used to prevent False Sharing.
    - Physical Core Pinning to eliminate inter-core latency and cache thrashing.
- **Memory Management:** Zero-allocation hot path (static allocation only).
- **PCI Access:** Raw register access via `/dev/mem` and `iopl(3)` for direct hardware interaction.

## Prerequisites
- **OS:** Linux (Kernel with `CAP_SYS_RAWIO` capabilities).
- **Compiler:** `g++` (supporting C++20).
- **Hardware:** x86_64 architecture with support for `rdtscp` (TSC synchronization).

## Build Instructions
1. Clean the build directory:
   ```bash
   make clean
   make

sudo ./hft_engine


"Implemented SPSC lock-free ring buffer for zero-copy packet passing."

Final Verification and Commit
Teh hft_engine engine is now:

Isolated: Using cache-line padding to prevent false sharing.

Lock-Free: Utilizing SPSC buffers for zero-mutex contention.

Pinned: Bound to specific physical cores to minimize OS scheduler interference.

Instrumented: Capturing cycle-accurate latency benchmarks on startup.