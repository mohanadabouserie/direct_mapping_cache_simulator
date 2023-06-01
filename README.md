# direct_mapping_cache_simulator

This project is a cache simulator implemented in C++. It simulates the behavior of a cache memory system and calculates cache hits, misses, hit ratio, miss ratio, and average memory access time (AMAT).

## Getting Started

To run the cache simulator, follow these instructions:

1. Ensure you have a C++ compiler installed on your system.
2. Copy the source code into a C++ file (e.g., `cache_simulator.cpp`).
3. Compile the code using the C++ compiler. For example, using `g++`:

   ```bash
   g++ cache_simulator.cpp -o cache_simulator
4. Run the compiled executable: ./cache_simulator

##Usage
Upon running the cache simulator, it will prompt you to provide the cache size, line size, and the number of access cycles needed to access the cache. Follow the on-screen instructions and enter the required values.

The simulator expects the memory addresses to be stored in a file named addresses.txt. Make sure to create this file and populate it with the memory addresses you want to simulate.

The simulator will display the cache state after each memory access and provide statistics such as the total number of accesses, hit ratio, miss ratio, and average memory access time (AMAT).
