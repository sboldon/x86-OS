bits 32
VID_MEM equ 0xb8000
WHITE_FG_BLACK_BG equ 0x0f

; Write null-terminated string in EBX to video memory.
print_string_prot:
    push edx
    push ebx
    push eax 
    mov edx, VID_MEM
    mov ah, WHITE_FG_BLACK_BG
  print_string_prot_start:
    mov al, [ebx]
    cmp al, 0                   ; Check for terminating null byte.
    je print_string_prot_end 
    mov [edx], ax               ; Write character and color to video memory cell.
    inc ebx                     ; Advance to address of next character in the string.
    add edx, 2                  ; Advance to address of next video memory cell.
    jmp print_string_prot_start
  print_string_prot_end:
    pop eax
    pop ebx
    pop edx
    ret

