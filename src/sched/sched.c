#include "sched/sched.h"
#include "libc/string.h"

static scheduler_t scheduler;
// Dispatch a new process while running within an interrupt routine.
static void dispatch_interrupt();
static void sched_interrupt_store(unsigned int idx);
static void sched_interrupt_replace(unsigned int idx);

// Define the round-robin time slice.
#define TIME_SLICE_CONSTANT 128

// Initialize the scheduler.
int sched_init() {
  memset(&scheduler, 0, sizeof(scheduler));
  return 0;
}

// Handle an interrupt.
void sched_interrupt(uint32_t counter, uint32_t stack_loc) {
  if (counter % TIME_SLICE_CONSTANT == 0) {
    dispatch_interrupt();
  }
}

// Dispatch a new process while running within an interrupt routine.
static void dispatch_interrupt() {
  for (int i = 0; i < MAX_PROCESSES; i++) {
    // Start at the last process, searching for something that is
    // 'READY' and not blocked.
    int idx = (scheduler.cur + i) % MAX_PROCESSES;
    if (scheduler.process_table[idx].state == PROCESS_READY) {
      // 1. The interrupted process state exists on the stack -> store
      // it to the 'cur' idx.
      sched_interrupt_store(scheduler.cur);
      // 2. Replace the interrupted process on the stack with the new one.
      sched_interrupt_replace(idx);
    }
  }
}

static void sched_interrupt_store(unsigned int idx) {
  
}

static void sched_interrupt_replace(unsigned int idx) {
  
}
