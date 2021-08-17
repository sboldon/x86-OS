; Tell assembler to use this address as base address of the section of the file.
; BIOS expects boot loader at this address.
org 0x7c00 

mov bp, 0x9000            ; Initialize stack
mov sp, bp

mov bx, RM_MSG
call print_string
call switch_to_prot_mode  ; Switch to 32 bit protected mode

%include "print.asm"      ; 16 bit real mode printing via BIOS interrupts
%include "gdt.asm"        ; GDT structure
%include "prot_mode.asm"  ; Routines to switch into protected mode and initialize segment registers.
%include "prot_print.asm" ; 32 bit protected mode printing via writing to video memory.

bits 32
PROT_MODE:
  mov ebx, PM_MSG 
  call print_string_prot
  jmp $

RM_MSG db "In 16 bit real mode", 0 
PM_MSG db "Switched to 32 bit protected mode", 0

; Insert number of 0 bytes needed to place 0xaa55 at bytes 511 and 512.
; $$ is section start, $$ - $ = current addr - section start = len of previous code.
times 510-($-$$) db 0
dw 0xaa55                 ; Tells BIOS to boot system.

