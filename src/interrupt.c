/**
 * Interrupt related routines.
 * https://wiki.osdev.org/Interrupt_Descriptor_Table#Structure_on_IA-32
 * 
 * Ethan Jones <erj0005@uah.edu>.
 */

#include "interrupt.h"
#include "term.h"

static void idt_set_descriptor(int idx, void* isr, uint8_t flags);

static idt_descriptor_t idt_desc;
__attribute__((aligned(0x10)))
static idt_gate_descriptor_t idt_table[256];

void interrupt_handler() {
 term_write("Got an interrupt.\n");
}

void setup_idt() {
 idt_desc.offset = (uint32_t) &idt_table[0];
 idt_desc.size   = (uint16_t) (sizeof(idt_gate_descriptor_t) * (255));
}

static void idt_set_descriptor(int idx, void* isr, uint8_t flags) {
 idt_gate_descriptor_t *desc = &idt_table[idx];
 desc->offset_1 = (uint32_t) isr & 0xFFFF;
 desc->offset_2 = (uint32_t) isr >> 16;
 desc->selector = 0x08; // TODO: This is the CS segment defined in the GDT.
 desc->type_attributes = flags;
 desc->zero = 0;
}
