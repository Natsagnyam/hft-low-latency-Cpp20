#ifndef MAIN_H
#define MAIN_H

// Define core assignments
#define WATCHDOG_CORE 0

// Core 0 (Physical Core 0)
#define PRODUCER_CORE 0
// Core 1 (Physical Core 1)
#define CONSUMER_CORE 2

#endif // MAIN_H

/* 
ASRock H81M motherboard is using a CPU with 2 physical cores and 4 logical threads (Hyper-Threading) as below:
natska@ASROCK-H81M:~$ lscpu -e=CPU,CORE,SOCKET
CPU CORE SOCKET
  0    0      0
  1    1      0
  2    0      0
  3    1      0
natska@ASROCK-H81M:~$ 
*/