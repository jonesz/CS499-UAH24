/**
 * Interrupt related routines.
 * https://wiki.osdev.org/Interrupt_Descriptor_Table#Structure_on_IA-32
 *
 * Ethan Jones <erj0005@uah.edu>.
 */

#include "interrupt.h"
#include "asm_tools.h"
#include "term.h"

#define MAX_IDT_ENTRIES 256
static void idt_set_descriptor(int idx, void *isr, uint8_t flags);

extern void *isr;
static idt_descriptor_t idt_desc;
__attribute__((
    aligned(0x10))) static idt_gate_descriptor_t idt_table[MAX_IDT_ENTRIES];

void interrupt_handler() { term_write("Got an interrupt.\n"); }
void key_handler(uint32_t scan_code) {
  term_format("Key Interrupt %x\n", &scan_code);
}

void setup_idt(void *isr_in, void *key_isr) {
  idt_desc.offset = (uint32_t)&idt_table[0];
  idt_desc.size = (uint16_t)(sizeof(idt_gate_descriptor_t) * (255) - 1);

  for (int i = 0; i < MAX_IDT_ENTRIES; i++) {

    if (i == (0x1F + 2)) {
      idt_set_descriptor(i, key_isr, 0x8E);
    } else {
      idt_set_descriptor(i, isr_in, 0x8E);
    }
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
  //term_write("Issue ICW1\n");
  outb(MPIC_CMD, ICW1_INIT | ICW1_ICW4);
  io_wait();
  outb(SPIC_CMD, ICW1_INIT | ICW1_ICW4);
  io_wait();

  // ICW2
  //term_write("Issue ICW2\n");
  outb(MPIC_DAT, 0x20);
  io_wait();
  outb(SPIC_DAT, 0x28);
  io_wait();

  // ICW3
  //term_write("Issue ICW3\n");
  outb(MPIC_DAT, 4);
  io_wait();
  outb(SPIC_DAT, 2);
  io_wait();

  // ICW4
  //term_write("Issue ICW4\n");
  outb(MPIC_DAT, ICW4_8086);
  io_wait();
  outb(SPIC_DAT, ICW4_8086);
  io_wait();

  // Set the IRQ mask
  // NOTE(BP): The mask I currently set only disables timer interrupts,
  // In my testing the timer was spamming interrupts and causing a headache,
  // So this will need to be changed when we actually want to use the timer.
  outb(MPIC_DAT, 0x1);
  outb(SPIC_DAT, 0x0);
  io_wait();
}
