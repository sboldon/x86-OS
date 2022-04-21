; Call main function in kernel code
bits 32

extern main
global _start
_start:
  call main
  jmp $
