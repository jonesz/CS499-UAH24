#ifndef SCHED_PROCESS
#define SCHED_PROCESS
#include <stdint.h>

#define PROCESS_UNUSED  0x0
#define PROCESS_RUNNING 0x1
#define PROCESS_READY   0x2
#define PROCESS_BLOCKED 0x3

typedef struct _x86_register_ctx {
  // GPRs.
  uint32_t eax;
  uint32_t ebx;
  uint32_t ecx;
  uint32_t esp;
  uint32_t ebp;
  uint32_t edi;
  uint32_t esi;
  uint32_t edx;

  // Segment Registers.
  uint32_t cs;
  uint32_t ds;
  uint32_t es;
  uint32_t fs;
  uint32_t gs;
  uint32_t ss;

  // EFLAGS.
  uint32_t EFLAGS;
} x86_register_ctx;

typedef struct _pcb_t {
  uint32_t eip;
  x86_register_ctx register_ctx;

  uint8_t state;
} pcb_t;

void print_pcb(pcb_t *pcb);

#endif // SCHED_PROCESS
