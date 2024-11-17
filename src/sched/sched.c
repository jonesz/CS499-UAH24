#include "sched/sched.h"
#include "libc/string.h"

static scheduler_t scheduler;
// Dispatch a new process while running within an interrupt routine.
static void dispatch_interrupt(uint32_t stack_loc);
static void sched_interrupt_store(unsigned int idx, uint32_t stack_loc);
static void sched_interrupt_replace(unsigned int idx, uint32_t stack_loc);

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
    dispatch_interrupt(stack_loc);
  }
}

// Dispatch a new process while running within an interrupt routine.
static void dispatch_interrupt(uint32_t stack_loc) {
  for (int i = 0; i < MAX_PROCESSES; i++) {
    // Start at the last process, searching for something that is
    // 'READY' and not blocked.
    int idx = (scheduler.cur + i) % MAX_PROCESSES;
    if (scheduler.process_table[idx].state == PROCESS_READY) {
      // 1. The interrupted process state exists on the stack -> store
      // it to the 'cur' idx.
      sched_interrupt_store(scheduler.cur, stack_loc);
      // 2. Replace the interrupted process on the stack with the new one.
      sched_interrupt_replace(idx, stack_loc);
      // 3. Set the current process to READY.
      scheduler.process_table[scheduler.cur].state = PROCESS_READY;
      // 4. Set the new process to RUNNING.
      scheduler.process_table[idx].state = PROCESS_RUNNING;
      return;
    }
  }
}

static void sched_interrupt_store(unsigned int idx, uint32_t stack_loc) {
  // EIP is at stack_loc (which is where the ISR `pushad` + 8).
  uint32_t eip = *(uint32_t *)(stack_loc + (4 * 2));
  // EFLAGS is right after that.
  uint32_t EFLAGS = *(uint32_t *)(stack_loc + (4 * 1));

  uint32_t eax = *(uint32_t *)(stack_loc - (4 * 0));
  uint32_t ecx = *(uint32_t *)(stack_loc - (4 * 1));
  uint32_t edx = *(uint32_t *)(stack_loc - (4 * 2));
  uint32_t ebx = *(uint32_t *)(stack_loc - (4 * 3));
  uint32_t esp = *(uint32_t *)(stack_loc - (4 * 4));
  uint32_t ebp = *(uint32_t *)(stack_loc - (4 * 5));
  uint32_t esi = *(uint32_t *)(stack_loc - (4 * 6));
  uint32_t edi = *(uint32_t *)(stack_loc - (4 * 7));

  scheduler.process_table[idx].eip = eip;
  scheduler.process_table[idx].register_ctx.EFLAGS = EFLAGS;
  scheduler.process_table[idx].register_ctx.eax = eax;
  scheduler.process_table[idx].register_ctx.ebx = ebx;
  scheduler.process_table[idx].register_ctx.ecx = ecx;
  scheduler.process_table[idx].register_ctx.edx = edx;
  scheduler.process_table[idx].register_ctx.esp = esp;
  scheduler.process_table[idx].register_ctx.ebp = ebp;
  scheduler.process_table[idx].register_ctx.esi = esi;
  scheduler.process_table[idx].register_ctx.edi = edi;
}

static void sched_interrupt_replace(unsigned int idx, uint32_t stack_loc) {
  uint32_t *eip = (uint32_t *)(stack_loc + (4 * 2));
  uint32_t *EFLAGS = (uint32_t *)(stack_loc + (4 * 1));
  uint32_t *eax = (uint32_t *)(stack_loc - (4 * 0));
  uint32_t *ecx = (uint32_t *)(stack_loc - (4 * 1));
  uint32_t *edx = (uint32_t *)(stack_loc - (4 * 2));
  uint32_t *ebx = (uint32_t *)(stack_loc - (4 * 3));
  uint32_t *esp = (uint32_t *)(stack_loc - (4 * 4));
  uint32_t *ebp = (uint32_t *)(stack_loc - (4 * 5));
  uint32_t *esi = (uint32_t *)(stack_loc - (4 * 6));
  uint32_t *edi = (uint32_t *)(stack_loc - (4 * 7));
  
  *eip = scheduler.process_table[idx].eip;
  *EFLAGS = scheduler.process_table[idx].register_ctx.EFLAGS;
  *eax = scheduler.process_table[idx].register_ctx.eax;
  *ecx = scheduler.process_table[idx].register_ctx.ecx;
  *edx = scheduler.process_table[idx].register_ctx.edx;
  *ebx = scheduler.process_table[idx].register_ctx.ebx;
  *esp = scheduler.process_table[idx].register_ctx.esp;
  *ebp = scheduler.process_table[idx].register_ctx.ebp;
  *esi = scheduler.process_table[idx].register_ctx.esi;
  *edi = scheduler.process_table[idx].register_ctx.edi;
}
