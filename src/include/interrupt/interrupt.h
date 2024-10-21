#ifndef INTERRUPT_INTERRUPT_H
#define INTERRUPT_INTERRUPT_H
#include <stdint.h>

// IDT Gate types
#define GATE_TYPE_TASK_GATE 0x5
#define GATE_TYPE_INTERRUPT_GATE_16 0x6
#define GATE_TYPE_TRAP_GATE_16 0x7
#define GATE_TYPE_INTERRUPT_GATE_32 0xE
#define GATE_TYPE_TRAP_GATE_32 0xF

// PIC Commands/Ports
// https://wiki.osdev.org/8259_PIC
#define MPIC_CMD 0x20
#define MPIC_DAT 0x21
#define SPIC_CMD 0xA0
#define SPIC_DAT 0xA1

#define MPIC_VOFF 0x20
#define SPIC_VOFF 0x28

#define ICW1_ICW4 0x01      /* Indicates that ICW4 will be present */
#define ICW1_SINGLE 0x02    /* Single (cascade) mode */
#define ICW1_INTERVAL4 0x04 /* Call address interval 4 (8) */
#define ICW1_LEVEL 0x08     /* Level triggered (edge) mode */
#define ICW1_INIT 0x10      /* Initialization - required! */

#define ICW4_8086 0x01       /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO 0x02       /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE 0x08  /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C /* Buffered mode/master */
#define ICW4_SFNM 0x10       /* Special fully nested (not) */

// Structure that stores the location of the IDT.
typedef struct idt_descriptor {
  uint16_t size;   //  0..15
  uint32_t offset; // 16..48
} __attribute__((packed)) idt_descriptor_t;

typedef struct idt_gate_descriptor {
  // Lower bits of the offset pointing to the ISR.
  uint16_t offset_1; // 0..15
  // "Segment Selector with multple fields which must point
  // to a valid code segment in the GDT."
  uint16_t selector; // 16..31
  uint8_t zero;      // 32..39
  // Composed of a "GATE_TYPE", "DPL" 2-bits, and "P" bit.
  uint8_t type_attributes; // 40..47
  // Upper bits of the offset pointing to the ISR.
  uint16_t offset_2; // 48..63
} __attribute__((packed)) idt_gate_descriptor_t;

void setup_idt(void *isr_in, void *key_isr);
void interrupt_handler(uint32_t int_num);
void init_pic();

#endif // INTERRUPT_INTERRUPT_H
