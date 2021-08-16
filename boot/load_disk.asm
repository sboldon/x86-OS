READ_ERR_MSG: db "Disk read failed", 0

; Load DH sectors at ES:BX from drive DL.
load_disk:
  push dx
  ; CHS addressing via Cylinder, Head, and Sector. Boot loader is on C = 0, H = 0, S = 1.
  mov ah, 2          ; Function request number (2 = Read Disk Sectors).
  mov al, dh         ; Number of sectors to read.
  mov ch, 0          ; Cylinder number.
  mov cl, 2          ; Sector number (in this case the one right after boot sector).
  mov dh, 0          ; Head number.
  int 0x13
  jc read_error      ; Carry flag set on a failed read.
  pop dx
  cmp dh, al         ; Confirm actual number of sectors loaded matches expected number. 
  jne read_error
  ret
  
read_error:
  mov bx, READ_ERR_MSG
  call print_string
  jmp $

