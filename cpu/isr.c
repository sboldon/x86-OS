#include "isr.h"
#include "idt.h"
#include "screen.h"

// The cause of each reserved interrupt.
static const char* exception_messages[] = {
  [0] = "Division By Zero",
  [1] = "Debug",
  [2] = "Non Maskable Interrupt",
  [3] = "Breakpoint",
  [4] = "Into Detected Overflow",
  [5] = "Out of Bounds",
  [6] = "Invalid Opcode",
  [7] = "No Coprocessor",
  [8] = "Double Fault",
  [9] = "Coprocessor Segment Overrun",
  [10] = "Bad TSS",
  [11] = "Segment Not Present",
  [12] = "Stack Fault",
  [13] = "General Protection Fault",
  [14] = "Page Fault",
  [15] = "Unknown Interrupt",
  [16] = "Coprocessor Fault",
  [17] = "Alignment Check",
  [18] = "Machine Check",
  [19] = "Reserved",
  [20] = "Reserved",
  [21] = "Reserved",
  [22] = "Reserved",
  [23] = "Reserved",
  [24] = "Reserved",
  [25] = "Reserved",
  [26] = "Reserved",
  [27] = "Reserved",
  [28] = "Reserved",
  [29] = "Reserved",
  [30] = "Reserved",
  [31] = "Reserved"
};


void isr_handler(struct registers regs) {
  if (regs.interrupt_num < 32) {
    screen_puts(exception_messages[regs.interrupt_num]);
    screen_puts(" Exception\n");
  }
}


void isr_install_reserved() {
  idt_set_reserved_gate(0, isr0);
  idt_set_reserved_gate(1, isr1);
  idt_set_reserved_gate(2, isr2);
  idt_set_reserved_gate(3, isr3);
  idt_set_reserved_gate(4, isr4);
  idt_set_reserved_gate(5, isr5);
  idt_set_reserved_gate(6, isr6);
  idt_set_reserved_gate(7, isr7);
  idt_set_reserved_gate(8, isr8);
  idt_set_reserved_gate(9, isr9);
  idt_set_reserved_gate(10, isr10);
  idt_set_reserved_gate(11, isr11);
  idt_set_reserved_gate(12, isr12);
  idt_set_reserved_gate(13, isr13);
  idt_set_reserved_gate(14, isr14);
  idt_set_reserved_gate(15, isr15);
  idt_set_reserved_gate(16, isr16);
  idt_set_reserved_gate(17, isr17);
  idt_set_reserved_gate(18, isr18);
  idt_set_reserved_gate(19, isr19);
  idt_set_reserved_gate(20, isr20);
  idt_set_reserved_gate(21, isr21);
  idt_set_reserved_gate(22, isr22);
  idt_set_reserved_gate(23, isr23);
  idt_set_reserved_gate(24, isr24);
  idt_set_reserved_gate(25, isr25);
  idt_set_reserved_gate(26, isr26);
  idt_set_reserved_gate(27, isr27);
  idt_set_reserved_gate(28, isr28);
  idt_set_reserved_gate(29, isr29);
  idt_set_reserved_gate(30, isr30);
  idt_set_reserved_gate(31, isr31);
}
