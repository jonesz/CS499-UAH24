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
} scheduler_t;

int sched_init();
void sched_kill(uint32_t stack_loc);
void sched_admit(uint32_t eip);
void sched_interrupt(uint32_t counter, uint32_t stack_loc);

#endif // SCHED_H
