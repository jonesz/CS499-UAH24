#ifndef INTERRUPT_H
#define INTERRUPT_H
#include <stdint.h>

#define GATE_TYPE_TASK_GATE 0x5
#define GATE_TYPE_INTERRUPT_GATE_16 0x6
#define GATE_TYPE_TRAP_GATE_16 0x7
#define GATE_TYPE_INTERRUPT_GATE_32 0xE
#define GATE_TYPE_TRAP_GATE_32 0xF

// Structure that stores the location of the IDT.
typedef struct idt_descriptor {
  uint16_t size;   //  0..15
  uint32_t offset; // 16..48
} idt_descriptor_t;

typedef struct idt_gate_descriptor {
  // Lower bits of the offset pointing to the ISR.
  uint16_t offset_1;       // 0..15
  // "Segment Selector with multple fields which must point
  // to a valid code segment in the GDT."
  uint16_t selector;       // 16..31
  uint8_t zero;            // 32..39
  // Composed of a "GATE_TYPE", "DPL" 2-bits, and "P" bit.
  uint8_t type_attributes; // 40..47
  // Upper bits of the offset pointing to the ISR.
  uint16_t offset_2;       // 48..63
} idt_gate_descriptor_t;

void setup_idt();
void interrupt_handler();

#endif // INTERRUPT_H
