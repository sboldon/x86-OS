extern isr_handler ; Implemented in `isr.c`

; The byte offset of the kernel's data segment descriptor in the GDT.
KERNEL_DATA_SEG equ 0x10

; Expects argument to be a pointer to the IDT descriptor.
global idt_flush
idt_flush:
  mov eax, [esp + 4]
  lidt [eax]
  ret


; The template for the start of an ISR that does not push an error code onto the stack.
%macro ISR_NOERRCODE 1
global isr%1
isr%1:
  push dword 0  ; Unused filler value taking the place of an error code so that stack is aligned.
  push dword %1
  jmp handle_interrupt
%endmacro


; The template for the start of an ISR that pushes an error code onto the stack.
%macro ISR_ERRCODE 1
global isr%1
isr%1:
  push dword %1
  jmp handle_interrupt
%endmacro


; Save processor state using `pusha` to store all general purpose registers on the stack, then load
; the kernel data segment descriptor and invoke C ISR handler. After handler has finished, restore
; the previous data segment descriptor and the processor state.
handle_interrupt:
  pusha                    ; Save EDI, ESI, EBP, ESP, EBX, EDX, ECX, and EAX.
  mov ax, ds
  push eax                 ; Save current data segment descriptor.

  mov ax, KERNEL_DATA_SEG  ; Load kernel data segment descriptor.
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov eax, isr_handler     ; Preserve EIP register across call.
  call eax                 ; The stack contains the values of a `registers` struct.

  pop eax                  ; Restore data segment descriptor.
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  popa
  add esp, 8               ; Pop error code and ISR number.
  iret                     ; Pop EIP, CS, EFLAGS, SS, and set ESP to user ESP.


ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE 8
ISR_NOERRCODE 9
ISR_ERRCODE 10
ISR_ERRCODE 11
ISR_ERRCODE 12
ISR_ERRCODE 13
ISR_ERRCODE 14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_ERRCODE 30
ISR_NOERRCODE 31
