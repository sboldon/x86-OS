#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// The byte offset of the kernel's code segment descriptor in the GDT.
#define KERNEL_CODE_SEG 0x8

// Populate the interrupt descriptor table (IDT) with the 32 built-in interrupt service routines
// (ISRs).
void idt_init();

// The interrupt descriptor stores the location of the IDT. It is loaded into the IDTR register
// using instruction `lidt`.
struct idt_desc {
  uint16_t limit;            // The address of the last valid byte in the table is `base + limit`.
  uint32_t base;             // Address of IDT.
} __attribute__((packed));

// An IDT entry specifies the task or procedure that is invoked in response to the corresponding
// interrupt vector.
// Layout of `attrs` byte:
// - P: Present bit; must be set to 1 for a valid descriptor.
// - Priv: The privilege levels that are allowed to invoke the interrupt with INT instruction.
// - Gate type: Defines the gate type of this interrupt descriptor as either:
//   0x5 - task gate
//   0x6 - 16 bit interrupt gate
//   0x7 - 16 bit trap gate
//   0xE - 32 bit interrupt gate
//   0xF -  32 bit trap gate
// ┌───┬───┬───┬───┬───┬───┬───┬───┐
// │ 7 │ 6 │ 5 │ 4 │ 3 │ 2 │ 1 │ 0 │
// ├───┼───┴───┼───┼───┴───┴───┴───┤
// │ P │  Priv │   │   Gate Type   │
// └───┴───────┴───┴───────────────┘
struct idt_gate_desc {
  uint16_t isr_lo;           // Lower half of the offset of the ISR.
  uint16_t code_seg;         // Points to code segment in GDT/LDT.
  uint8_t reserved;
  uint8_t attrs;
  uint16_t isr_hi;           // Upper half of ISR offset.
} __attribute__((packed));

enum idt_gate_type {
  IDT_TASK_GATE = 0x5,
  IDT_INTERRUPT_GATE_16 = 0x6,
  IDT_TRAP_GATE_16 = 0x7,
  IDT_INTERRUPT_GATE_32 = 0xE,
};

// Initialize the gate descriptor of a reserved ISR.
void idt_set_reserved_gate(uint8_t num, const void *isr_addr);

#endif
