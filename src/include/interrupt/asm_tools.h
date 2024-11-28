#ifndef ASM_TOOLS_H
#define ASM_TOOLS_H

#include <stdint.h>

static inline void outb(uint16_t port, uint8_t dat) {
  __asm__ volatile("outb %b0, %w1" ::"a"(dat), "Nd"(port) : "memory");
}
static inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  __asm__ volatile("inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
  return ret;
}

static inline void io_wait() { outb(0x80, 0); }

static inline uint32_t get_esp() {
  uint32_t ret;
  __asm__ volatile("mov %%esp, %d0" : "=a"(ret));
  return ret;
}
static inline uint32_t get_eflags() {
  uint32_t ret;
  asm volatile ("pushf;\
                                    pop %%eax;       \
                                    mov %%eax, %0;"  \
                                    :"=m" (ret)     \
                                    ); 
  return ret;
}
#endif
