#ifndef ISR_H
#define ISR_H

#include <stdint.h>

// Denotes a 32 bit interrupt that requires kernel privilege to invoke.
#define ISR_GATE_TYPE (0x80 | IDT_INTERRUPT_GATE_32)

// The contents of the stack right before `isr_handler` is called.
struct registers {
  uint32_t ds;                                      // Data segment descriptor.
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  // General purpose regs.
  uint32_t interrupt_num;
  uint32_t error_code;
  uint32_t eip, cs, eflags, user_esp, ss;           // Automatically pushed on stack by `int` instruction.
};

// Initialize the IDT gate descriptor of each reserved ISR.
void isr_install_reserved();

// Invoked by an ISR to display its exception message.
void isr_handler(struct registers regs);

// Handling routines for reserved IA-32 interrupts and exceptions. The functions are implemented in
// `interrupts.asm`.
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

#endif
