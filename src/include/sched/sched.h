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
  // The process that is blocking the specific index.
  uint32_t blocker[MAX_PROCESSES];
} scheduler_t;

int sched_init();
uint32_t sched_kill(uint32_t stack_loc);
void sched_admit(uint32_t eip);
uint32_t sched_admit_args(uint32_t eip, uint32_t argc, char** argv);
void sched_interrupt(uint32_t counter, uint32_t stack_loc);
void sched_block(uint32_t stack_loc, uint32_t blocker);
void sched_unblock(uint32_t blocker);
uint32_t sched_who_is_running();

#endif // SCHED_H
