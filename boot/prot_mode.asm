bits 16
switch_to_prot_mode:
  cli                          ; Disable interrupts.
  lgdt [gdt_desc]              ; Load GDT via its descriptor.
  mov eax, cr0                 ; Switch to protected mode via setting first bit of CR0 register.
  or al, 1
  mov cr0, eax
  jmp CODE_SEG:init_prot_mode  ; Flush CPU pipeline via a far jump to ensure smooth mode transition.

bits 32
; Initialize segment registers and stack use GDT data segment.
init_prot_mode:
  mov ax, DATA_SEG
  mov ds, ax 
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ebp, 0x90000 ; Beginning of stack
  mov esp, ebp
  jmp PROT_MODE 

