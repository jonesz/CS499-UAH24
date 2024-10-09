#include <stdint.h>
#include <stdio.h>

int main(void) {
  FILE *f = fopen("interrupt/isrs.meta.asm", "w");
  for (uint32_t i = 0; i < 256; i++) {
    if (i < 2) {
      fprintf(f, "global isr%d\n", i);
    }
    fprintf(f, "isr%d:\n", i);
    fprintf(f, "\tcli\n");
    fprintf(f, "\tpushad\n");
    fprintf(f, "\tmov eax, %d\n", i);
    fprintf(f, "\tpush eax\n");
    fprintf(f, "\tcld\n");
    fprintf(f, "\textern interrupt_handler\n");
    fprintf(f, "\tcall interrupt_handler\n");
    fprintf(f, "\tpop eax\n");
    fprintf(f, "\tpopad\n");
    fprintf(f, "\tsti\n");
    fprintf(f, "\tiret\n");
    fprintf(f, "\n");
  }
}
