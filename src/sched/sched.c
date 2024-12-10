#include "sched/sched.h"
#include "libc/string.h"
#include "mem/fixed_alloc.h"
#include "syscalls/syscalls.h"
#include "vid/term.h"

static scheduler_t scheduler;
// Dispatch a new process while running within an interrupt routine.
static void dispatch_interrupt(uint32_t stack_loc);
static void sched_interrupt_store(unsigned int idx, uint32_t stack_loc);
static void sched_interrupt_replace(unsigned int idx, uint32_t stack_loc);

static void exit_stub();

// Define the round-robin time slice.
#define TIME_SLICE_CONSTANT 4
#define INT_ENABLE_BIT 512

// Initialize the scheduler.
int sched_init() {
  memset(&scheduler, 0, sizeof(scheduler));
  return 0;
}

void sched_admit(uint32_t eip) {
  // Save eflags immediately
  uint32_t eflags;
  asm volatile("pushfl;\
                popl %%eax;       \
                movl %%eax, %0;"
               : "=m"(eflags));

  for (int i = 0; i < MAX_PROCESSES; i++) {
    if (scheduler.process_table[i].state == PROCESS_UNUSED) {
      scheduler.process_table[i].eip = eip;

      uint32_t stack = (uint32_t)fixed_alloc();
      scheduler.process_table[i].stack_addr = stack;

      // Without some `libcrt` type linker where `exit()` is linked at the end
      // of `main`, we need a way to trigger de-allocation of the process. Place
      // an "exit stub" at the end which after return, the program should fall
      // to (this is what BP wanted; I disagree).
      *(uint32_t *)stack = (uint32_t)&exit_stub;

      scheduler.process_table[i].register_ctx.ebp = stack;
      scheduler.process_table[i].register_ctx.esp = stack;

      scheduler.process_table[i].register_ctx.cs = 0x8; // TODO: fix this later.
      scheduler.process_table[i].state = PROCESS_READY;

      // Set the interrupt enable bit for the process
      scheduler.process_table[i].register_ctx.EFLAGS = eflags;
      return;
    }
  }
}

// NOTE: Can only be called within an interrupt.
uint32_t sched_admit_args(uint32_t eip, uint32_t argc, char **argv) {
  // Save eflags immediately
  uint32_t eflags;
  asm volatile("pushfl;\
                popl %%eax;       \
                movl %%eax, %0;"
               : "=m"(eflags));

  for (int i = 0; i < MAX_PROCESSES; i++) {
    if (scheduler.process_table[i].state == PROCESS_UNUSED) {
      scheduler.process_table[i].eip = eip;

      uint32_t stack = (uint32_t)fixed_alloc();
      scheduler.process_table[i].stack_addr = stack;
      scheduler.process_table[i].register_ctx.ebp = stack;

      // TODO: We don't have process isolation, so we're not going to care about
      // copies over the stack.
      *(uint32_t *)stack = (uint32_t)argv;
      stack -= 4;

      *(uint32_t *)stack = argc;
      stack -= 4;

      // Without some `libcrt` type linker where `exit()` is linked at the end
      // of `main`, we need a way to trigger de-allocation of the process. Place
      // an "exit stub" at the end which after return, the program should fall
      // to (this is what BP wanted; I disagree).

      // We place this here because after an 'asm' call, we expect the return
      // address to be placed on the stack. So it's argv, argc, return address.
      *(uint32_t *)stack = (uint32_t)&exit_stub;

      scheduler.process_table[i].register_ctx.esp = stack;
      scheduler.process_table[i].register_ctx.cs = 0x8; // TODO: fix this later.
      scheduler.process_table[i].state = PROCESS_READY;

      // Set the interrupt enable bit for the process
      scheduler.process_table[i].register_ctx.EFLAGS = eflags;
      return i;
    }
  }
}

// Called from a `Sys_Exit` interrupt.
uint32_t sched_kill(uint32_t stack_loc) {
  for (int i = 0; i < MAX_PROCESSES; i++) {
    if (scheduler.process_table[i].state == PROCESS_RUNNING) {
      fixed_free((void *)scheduler.process_table[i]
                     .stack_addr); // De-allocate the stack.
      scheduler.process_table[i].state = PROCESS_UNUSED;
      dispatch_interrupt(stack_loc); // Place a new process onto the stack.
      return i;
    }
  }
}

// Handle an interrupt.
void sched_interrupt(uint32_t counter, uint32_t stack_loc) {
  if (counter % TIME_SLICE_CONSTANT == 0) {
    dispatch_interrupt(stack_loc);
  }
}

// Dispatch a new process while running within an interrupt routine.
static void dispatch_interrupt(uint32_t stack_loc) {
  int cur = -1;
  for (int i = 0; i < MAX_PROCESSES; i++) {
    if (scheduler.process_table[i].state == PROCESS_RUNNING) {
      cur = i;
      break;
    }
  }

  if (cur == -1) {
    // If we couldn't find a process that is running, we're at program startup.
    // Throw the first ready process on the stack.
    for (int i = 0; i < MAX_PROCESSES; i++) {
      if (scheduler.process_table[i].state == PROCESS_READY) {
        sched_interrupt_replace(i, stack_loc);
        scheduler.process_table[i].state = PROCESS_RUNNING;
        uint32_t eip = *(uint32_t *)(stack_loc + 4);
        break;
      }
    }
    return;
  }

  for (int i = 0; i < MAX_PROCESSES; i++) {
    // Start at the last process, searching for something that is
    // 'READY' and not blocked.
    int idx = (cur + i) % MAX_PROCESSES;
    if (scheduler.process_table[idx].state == PROCESS_READY) {
      // 1. The interrupted process state exists on the stack -> store
      // it to the 'cur' idx.
      sched_interrupt_store(cur, stack_loc);
      // 2. Replace the interrupted process on the stack with the new one.
      sched_interrupt_replace(idx, stack_loc);
      // 3. Set the previous process to READY.
      scheduler.process_table[cur].state = PROCESS_READY;
      // 4. Set the next process to RUNNING.
      scheduler.process_table[idx].state = PROCESS_RUNNING;
      return;
    }
  }
}

// Block the current process.
void sched_block(uint32_t stack_loc, uint32_t waiting) {
  // Find the current process that's running.
  int cur;
  for (int i = 0; i < MAX_PROCESSES; i++) {
    if (scheduler.process_table[i].state == PROCESS_RUNNING) {
      cur = i;
      break;
    }
  }

  // Swap to another process.
  dispatch_interrupt(stack_loc);

  // Set the old process as blocked.
  scheduler.process_table[cur].state = PROCESS_BLOCKED;
  scheduler.block[cur] = waiting;
  return;
}

// Go ahead and unblock everything.
void sched_unblock(uint32_t waiting) {
  for (int i = 0; i < MAX_PROCESSES; i++) {
    if (scheduler.process_table[i].state == PROCESS_BLOCKED && scheduler.block[i] == waiting) {
      scheduler.process_table[i].state = PROCESS_READY;
    }
  }
  return;
}

static void sched_interrupt_store(unsigned int idx, uint32_t stack_loc) {
  uint32_t EFLAGS = *(uint32_t *)(stack_loc + 12);
  uint32_t cs = *(uint32_t *)(stack_loc + 8);
  uint32_t eip = *(uint32_t *)(stack_loc + 4);

  uint32_t eax = *(uint32_t *)(stack_loc - (4 * 0));
  uint32_t ecx = *(uint32_t *)(stack_loc - (4 * 1));
  uint32_t edx = *(uint32_t *)(stack_loc - (4 * 2));
  uint32_t ebx = *(uint32_t *)(stack_loc - (4 * 3));
  uint32_t esp = *(uint32_t *)(stack_loc - (4 * 4));
  uint32_t ebp = *(uint32_t *)(stack_loc - (4 * 5));
  uint32_t esi = *(uint32_t *)(stack_loc - (4 * 6));
  uint32_t edi = *(uint32_t *)(stack_loc - (4 * 7));

  scheduler.process_table[idx].eip = eip;
  scheduler.process_table[idx].register_ctx.cs = cs;
  scheduler.process_table[idx].register_ctx.EFLAGS = EFLAGS;
  scheduler.process_table[idx].register_ctx.eax = eax;
  scheduler.process_table[idx].register_ctx.ebx = ebx;
  scheduler.process_table[idx].register_ctx.ecx = ecx;
  scheduler.process_table[idx].register_ctx.edx = edx;

  // NOTE(Britton): Add 12 to "pop" eip, cs, and eflags from the old process'
  // stack
  scheduler.process_table[idx].register_ctx.esp = esp + 12;
  scheduler.process_table[idx].register_ctx.ebp = ebp;
  scheduler.process_table[idx].register_ctx.esi = esi;
  scheduler.process_table[idx].register_ctx.edi = edi;
}

static void sched_interrupt_replace(unsigned int idx, uint32_t stack_loc) {
  uint32_t *EFLAGS = (uint32_t *)(stack_loc + 12);
  uint32_t *cs = (uint32_t *)(stack_loc + 8);
  uint32_t *eip = (uint32_t *)(stack_loc + 4);

  uint32_t *eax = (uint32_t *)(stack_loc - (4 * 0));
  uint32_t *ecx = (uint32_t *)(stack_loc - (4 * 1));
  uint32_t *edx = (uint32_t *)(stack_loc - (4 * 2));
  uint32_t *ebx = (uint32_t *)(stack_loc - (4 * 3));
  uint32_t *esp = (uint32_t *)(stack_loc - (4 * 4));
  uint32_t *ebp = (uint32_t *)(stack_loc - (4 * 5));
  uint32_t *esi = (uint32_t *)(stack_loc - (4 * 6));
  uint32_t *edi = (uint32_t *)(stack_loc - (4 * 7));

  *eip = scheduler.process_table[idx].eip;
  *cs = scheduler.process_table[idx].register_ctx.cs;
  *EFLAGS = scheduler.process_table[idx].register_ctx.EFLAGS;
  *eax = scheduler.process_table[idx].register_ctx.eax;
  *ecx = scheduler.process_table[idx].register_ctx.ecx;
  *edx = scheduler.process_table[idx].register_ctx.edx;
  *ebx = scheduler.process_table[idx].register_ctx.ebx;

  // Subtract 12 to make room on the stack for iret arguments
  *esp = scheduler.process_table[idx].register_ctx.esp - 12;
  *ebp = scheduler.process_table[idx].register_ctx.ebp;
  *esi = scheduler.process_table[idx].register_ctx.esi;
  *edi = scheduler.process_table[idx].register_ctx.edi;

  // NOTE(Britton): "push" eip, cs, and eflags to the incoming process' stack so
  // that iret works
  esp = (void *)*esp;
  *(esp + 2) = *EFLAGS;
  *(esp + 1) = *cs;
  *esp = *eip;
}

static void exit_stub() {
  exit(); // Call the exit() syscall.
}
