#ifndef SCHED_H
#define SCHED_H
#include <stdint.h>
#include "process.h"

// Rather than utilize a linked-list structure, we'll hard cap
// the process limit at 256.
#define MAX_PROCESSES 256

typedef struct _scheduler_t {
  // A set of running processes.
  pcb_t process_table[MAX_PROCESSES];
  // The index of the current `RUNNING` process.
  uint8_t cur;
} scheduler_t;

int sched_init();
void sched_interrupt(uint32_t counter);

#endif // SCHED_H
