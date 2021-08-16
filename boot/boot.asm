; Tell assembler to use this address as base address of the section of the file.
; BIOS expects boot loader at this address.
org 0x7c00 

mov [BOOT_DRIVE], dl      ; BIOS will populate DL with drive number on start up.
mov bx, 0x7e00            ; Load at address 0x7e00 = 0x7c00 + sizeof(sector).
mov dh, 2                 ; Load two sectors.
call load_disk
mov dx, [0x7e00]          ; First word of sector loaded into memory.
call print_hex
mov dx, [0x7e00 + 512]
call print_hex

jmp $                     ; Jump to current memory address. (Infinite loop)

%include "print.asm"
%include "load_disk.asm"

BOOT_DRIVE: db 0

; Insert number of 0 bytes needed to place 0xaa55 at bytes 511 and 512.
; $$ is section start, $$ - $ = current addr - section start = len of previous code.
times 510-($-$$) db 0
dw 0xaa55                 ; Tells BIOS to boot system.

; Fill next two sectors of disk
times 512 db 'A'          
times 512 db 'B'

