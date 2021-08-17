; Segmentation register configuration based on Intel's basic flat model.
; Two overlapping segments, code and data, span the entire 4GB of addressable memory. 
gdt_start:

; Global descriptor table (GDT) requires first 8 byte segment descriptor to be null.
null_desc:
  dd 0
  dd 0

; With granularity bit set, the segment limit of 0xfffff is multiplied by 4K which results in the segment being capable of addressing 4GB. 
code_desc:
  dw 0xffff             ; Segment limit: 0-15
  dw 0                  ; Base limit: 0-15
  db 0                  ; Base limit: 16-23
; Segment type: 4, 
;   - accessed: 1
;   - readable: 1
;   - conforming: 1
;   - code: 1
; descriptor type: 1, 
; privilege level: 2, 
; segment present: 1
  db 10011010b
; Segment limit: 16-19, 
; available: 1, 
; 64 bit code segment: 1, 
; default operation size: 1, 
; granularity: 1 
  db 11001111b
  db 0                  ; Base limit: 24-31

; Same configuration as code descriptor except for the 4 bits of segment type.
data_desc:
  dw 0xffff
  dw 0
  db 0
; Segment type: 4, 
;   - accessed: 1
;   - writable: 1
;   - expand down: 1
;   - code: 1
  db 10010010b
  db 11001111b
  db 0
gdt_end:            ; Allows assembler to calculate GDT size.


gdt_desc:
  dw gdt_end - gdt_start - 1 ; Size of GDT, one less than actual size.
  dd gdt_start               ; Address of GDT

CODE_SEG equ code_desc - gdt_start
DATA_SEG equ data_desc - gdt_start

