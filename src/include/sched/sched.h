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

// Within the context of this file, `stack_loc` corresponds to the current location EIP points to;
// this is needed so the scheduler can address the `pushad` registers for saving into the process
// context.

/* Initialize the scheduler. */
int sched_init();
/* Kill the currently running process (passed a pointer to the current stack location). */
uint32_t sched_kill(uint32_t stack_loc);
/* Admit a process with the `eip` pointing to the start address of the code. */
void sched_admit(uint32_t eip);
/* Admit a process with the `eip` pointing to the start address of the code; additionally,
 * populate the stack with the appropriate argc and argv values. */
uint32_t sched_admit_args(uint32_t eip, uint32_t argc, char** argv);
/* Called on timer interrupts to the scheduler. */
void sched_interrupt(uint32_t counter, uint32_t stack_loc);
/* Block the currently running process, recording that we're blocked by `blocker`. */
void sched_block(uint32_t stack_loc, uint32_t blocker);
/* Unblock all processes that were blocked by `blocker`. */
void sched_unblock(uint32_t blocker);
/* Return the ID of the process that is currently running.*/
uint32_t sched_who_is_running();

#endif // SCHED_H
