; 16-bit real mode printing routines.

; Empty hex string template.
HEX_OUT:
  db '0x0000', 0

; Print number in DX.
print_hex:
    push bx
    push ax
    mov bx, HEX_OUT + 5 ; Address of LSB in hex string template.
  start:
    test dx, dx
    je fin
    mov al, dl
    and al, 0xf       ; Grab least significant 4 bits.
    cmp al, 9
    jle le9
    add al, 87        ; Turn digits a-f to characters 'a' - 'f'.
    jmp gt9
  le9:
    add al, 48        ; Turn digits 0-9 to characters '0' - '9'.
  gt9:
    mov [bx], al      ; Set character of hex string.
    dec bx
    shr dx, 4
    jmp start
  fin:
    mov bx, HEX_OUT
    call print_string
    pop ax
    pop bx
    ret

; Print null-terminated string in BX.
print_string:
    push bx
    mov ah, 0x0e       ; Put BIOS in TTY mode.
  start_b:
    mov al, [bx]
    cmp al, 0
    je fin_b
    int 0x10
    inc bx
    jmp start_b
  fin_b:
    pop bx
    ret

; Set

; Move cursor to row DH and col DL
update_cursor:
    mov ah, 0x02 ; Set cursor position
    int 0x10

