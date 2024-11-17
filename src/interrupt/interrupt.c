/**
 * Interrupt related routines.
 * https://wiki.osdev.org/Interrupt_Descriptor_Table#Structure_on_IA-32
 *
 * Ethan Jones <erj0005@uah.edu>.
 */

#include "interrupt/interrupt.h"
#include "interrupt/asm_tools.h"
#include "interrupt/isr.h"
#include "sched/sched.h"
#include "vid/term.h"

// TODO: At some point, this should be shared mem between the scheduler and the timer.
uint32_t counter = 0;

#define MAX_IDT_ENTRIES 256
static void idt_set_descriptor(int idx, void *isr, uint8_t flags);
static void key_handler(uint32_t int_num);
static void timer_handler(uint32_t stack_pos);

extern void *isr;
static idt_descriptor_t idt_desc;
__attribute__((
    aligned(0x10))) static idt_gate_descriptor_t idt_table[MAX_IDT_ENTRIES];

void interrupt_handler(uint32_t int_num, uint32_t stack_pos) {
  switch (int_num) {
  case KEYBOARD_ISR: {
    term_format("stack_pos: %x\n", &stack_pos);
    key_handler(int_num);
  } break;

  case TIMER_ISR:
     timer_handler(stack_pos);
     break;

  default: {
     term_format("Got interrupt %x.\n", &int_num);
  }
  }
}

void key_handler(uint32_t int_num) {
  // TODO(BP): All keyboard I/O should go through a driver
  // Get scan code from keyboard
  uint32_t scan_code = inb(0x60);
  term_format("Key Interrupt %x\n", &scan_code);
  // Tell PIC that the IRQ was handled
  outb(MPIC_CMD, 0x20);
}

void timer_handler(uint32_t stack_loc) {
  counter += 1;
  sched_interrupt(counter, stack_loc);
  outb(MPIC_CMD, 0x20);
}

void setup_idt(void *isr0, void *isr1) {
  idt_desc.offset = (uint32_t)&idt_table[0];
  idt_desc.size = (uint16_t)(sizeof(idt_gate_descriptor_t) * (255) - 1);
  // The ISR length is uniform, so we can calculate the next isr address via
  // offset: see below.
  int isr_len = isr1 - isr0;
  for (int i = 0; i < MAX_IDT_ENTRIES; i++) {
    // TODO: This isn't safe at all and is ghetto as hell -- relying on the
    // compiler to lay certain .text in a certain way is stupid; moreover,
    // making ISR size non-uniform will break this without making a sound. The
    // more intelligent way is to just pass an arr of 256 addresses.
    idt_set_descriptor(i, isr0 + i * isr_len, 0x8E);
  }

  __asm__ volatile("lidt %0" : : "m"(idt_desc));
}

static void idt_set_descriptor(int idx, void *isr, uint8_t flags) {
  idt_gate_descriptor_t *desc = &idt_table[idx];
  desc->offset_1 = (uint32_t)isr & 0xFFFF;
  desc->offset_2 = (uint32_t)isr >> 16;
  desc->selector = 0x08; // TODO: This is the CS segment defined in the GDT.
  desc->type_attributes = flags;
  desc->zero = 0;
}

// https://wiki.osdev.org/8259_PIC
void init_pic() {
  // ICW1
  // term_write("Issue ICW1\n");
  outb(MPIC_CMD, ICW1_INIT | ICW1_ICW4);
  io_wait();
  outb(SPIC_CMD, ICW1_INIT | ICW1_ICW4);
  io_wait();

  // ICW2
  // term_write("Issue ICW2\n");
  outb(MPIC_DAT, 0x20);
  io_wait();
  outb(SPIC_DAT, 0x28);
  io_wait();

  // ICW3
  // term_write("Issue ICW3\n");
  outb(MPIC_DAT, 4);
  io_wait();
  outb(SPIC_DAT, 2);
  io_wait();

  // ICW4
  // term_write("Issue ICW4\n");
  outb(MPIC_DAT, ICW4_8086);
  io_wait();
  outb(SPIC_DAT, ICW4_8086);
  io_wait();

  // Set the IRQ mask
  // NOTE(BP): The mask I currently set only disables timer interrupts,
  // In my testing the timer was spamming interrupts and causing a headache,
  // So this will need to be changed when we actually want to use the timer.
  outb(MPIC_DAT, 0x0);;
  outb(SPIC_DAT, 0x0);
  io_wait();
}
