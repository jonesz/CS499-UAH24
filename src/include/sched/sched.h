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
void sched_admit(uint32_t eip, uint32_t argc, char** argv, uint32_t argv_is_present);
void sched_interrupt(uint32_t counter, uint32_t stack_loc);
void sched_block(uint32_t stack_loc);
void sched_unblock();

#endif // SCHED_H
