#include "sched/process.h"
#include "vid/term.h"

void print_pcb(pcb_t *pcb) {
  term_format("EIP: %x\n", &pcb->eip);
  term_format("EAX: %x\n", &pcb->register_ctx.eax);
  term_format("EBX: %x\n", &pcb->register_ctx.ebx);
  term_format("ECX: %x\n", &pcb->register_ctx.ecx);
  term_format("EDX: %x\n", &pcb->register_ctx.edx);
  term_format("EFLAGS: %x\n", &pcb->register_ctx.EFLAGS);
  term_write("\n");
}
