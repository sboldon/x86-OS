#include "idt.h"
#include "isr.h"
#include <stdint.h>

// Asm routine to load new IDT. Implemented in `interrupts.asm`.
extern void idt_flush(struct idt_desc *);

// The IDT associates a handling procedure with each interrupt vector or exception. The alignment
// helps to maximize cache efficiency.
__attribute__((aligned (0x10)))
static struct idt_gate_desc idt[256];

// Pointer to IDT.
static struct idt_desc idt_reg;

// Populate the interrupt gate descriptor at index `num` with the provided arguments.
static void idt_set_gate(uint8_t num, const void *const isr, uint8_t attrs) {
  idt[num].isr_lo = (uint32_t)isr & 0xFFFF;
  idt[num].isr_hi = ((uint32_t)isr >> 16) & 0xFFFF;
  idt[num].code_seg = KERNEL_CODE_SEG;
  idt[num].reserved = 0;
  idt[num].attrs = attrs;
}


void idt_set_reserved_gate(uint8_t num, const void *const isr) {
  idt_set_gate(num, isr, ISR_GATE_TYPE);
}


void idt_init() {
  idt_reg.base = (uint32_t)idt;
  idt_reg.limit = 256 * sizeof(struct idt_gate_desc) - 1;
  isr_install_reserved();
  idt_flush(&idt_reg);
}
