; Tell assembler to use this address as base address of the section of the file.
; BIOS expects boot loader at this address.
org 0x7c00 

KERNEL_OFFSET equ 0x1000  ; Memory location that kernel will be loaded

mov [BOOT_DRIVE], dl      ; BIOS will populate DL with drive number on start up.

mov bp, 0x9000            ; Initialize stack.
mov sp, bp

mov bx, RM_MSG
call print_string
call load_kernel          ; Load kernel into memory.
call switch_to_prot_mode  ; Switch into 32 bit protected mode.

%include "print.asm"      ; 16 bit real mode printing via BIOS interrupts.
%include "load_disk.asm"  ; Reading sectors of the disk via BIOS.
%include "gdt.asm"        ; GDT structure.
%include "prot_mode.asm"  ; Routines to switch into protected mode and initialize segment registers.
%include "prot_print.asm" ; 32 bit protected mode printing via writing to video memory.

bits 16 
load_kernel:
  mov bx, LOAD_MSG 
  call print_string
  mov dh, 1 
  mov bx, KERNEL_OFFSET
  call load_disk          ; Load DH sectors from disk at KERNEL_OFFSET address
  ret

bits 32
PROT_MODE:                ; Operating in protected mode
  mov ebx, PM_MSG 
  call print_string_prot
  call KERNEL_OFFSET      ; Jump to address of loaded kernel
  jmp $

RM_MSG     db "Starting boot in 16 bit real mode", 0 
LOAD_MSG   db "Loading kernel", 0
PM_MSG     db "Switched to 32 bit protected mode", 0
BOOT_DRIVE db 0

; Insert number of 0 bytes needed to place 0xaa55 at bytes 511 and 512.
; $$ is section start, $$ - $ = current addr - section start = len of previous code.
times 510-($-$$) db 0
dw 0xaa55                 ; Tells BIOS to boot system.

