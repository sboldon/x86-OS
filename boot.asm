; Print the alphabet
  mov ah, 0x0e ; Switch BIOS to teletype mode
  mov al, 'A'
loop:
  int 0x10     ; Interrupt to print
  inc al
  cmp al, 'Z'
  jle loop

jmp $ ; Jump to current memory address.

; Insert number of 0 bytes needed to place 0xaa55 at bytes 511 and 512.
; $$ is section start, $$ - $ = current addr - section start = len of previous code.
times 510-($-$$) db 0 512
dw 0xaa55                 ; Tells BIOS to boot system.



