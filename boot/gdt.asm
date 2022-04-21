; GDT Segment descriptor
; - Base: A 32 bit address denoting the beginning of the segment.
; - Limit: A 20 bit value indicating the maximum addressable unit. The unit is either bytes or 4KiB
;   pages depending on the granularity bit. With page granularity and the largest possible limit of
;   0xFFFFF, the segment spans the entire 4GB of addressable memory.
; - Flags:
;   - Gr: Granularity bit. If clear, the limit unit is bytes. If set, the limit unit is 4KiB blocks.
;   - Sz: Size. If clear, descriptor is for a 16 bit prot mode segment. If set, descriptor is for a
;     32 bit prot mode segment.
;   - L: Long-mode code. If set then size must also be set and the descriptor defines a 64 bit code
;   segment. Should be clear for all other segment types (code & data)
; - Access byte:
;   - P: Present bit; must be set if segment is valid.
;   - Priv: CPU privilege level of the segment.
;   - S: Descriptor type. If clear, the descriptor defines a system segment such as a task state
;   segment. If set, descriptor denotes a code or data segment.
;   - Ex: Executable bit. If clear, the segment is a data segment. If set, the segment is a code
;   segment.
;   - DC:
;     - If data segment, direction bit. If clear, the segment grows up. If set, the segment grows
;     down, requiring an offset that is greater than the segment limit.
;     - If code segment, conforming bit. If clear, executing the segment's code requires the
;     privilege level of the segment. If set, the segment's code can be executed from an equal
;     or lower privilege level. For example, a conforming segment with privilege 2 can be executed
;     from privilege levels 2 or 3.
;   - RW:
;     - If data segment, writeable bit. If set, the segment is writeable.
;     - If code segment, readable bit. If set, the segment is readable.
;   - Ac: Access bit; CPU will automatically set when segment is accessed.
; ┌────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬─────┬────┬────┬────┐
; │ 63 │ 62 │ 61 │ 60 │ 59 │ 58 │ 57 │ 56 │ 55 │ 54 │ 53 │ 52 │ 51  │ 50 │ 49 │ 48 │
; ├────┴────┴────┴────┴────┴────┴────┴────┼────┼────┼────┼────┼─────┴────┴────┴────┤
; │         Base Address (24-31)          │ Gr │ Sz │ L  │    │ Seg. Limit (16-19) │
; ├────┬────┬────┬────┬────┬────┬────┬────┼────┼────┼────┼────┼─────┬────┬────┬────┤
; │ 47 │ 46 │ 45 │ 44 │ 43 │ 42 │ 41 │ 40 │ 39 │ 38 │ 37 │ 36 │ 35  │ 34 │ 33 │ 32 │
; ├────┼────┴────┼────┼────┼────┼────┼────┼────┴────┴────┴────┴─────┴────┴────┴────┤
; │ P  │  Priv   │ S  │ Ex │ DC │ RW │ Ac │          Base Address (16-23)          │
; ├────┼────┬────┼────┼────┼────┼────┼────┼────┬────┬────┬────┬─────┬────┬────┬────┤
; │ 31 │ 30 │ 29 │ 28 │ 27 │ 26 │ 25 │ 24 │ 23 │ 22 │ 21 │ 20 │ 19  │ 18 │ 17 │ 16 │
; ├────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴─────┴────┴────┴────┤
; │                              Base Address (0-15)                               │
; ├────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬────┬─────┬────┬────┬────┤
; │ 15 │ 14 │ 13 │ 12 │ 11 │ 10 │ 9  │ 8  │ 7  │ 6  │ 5  │ 4  │ 3   │ 2  │ 1  │ 0  │
; ├────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴────┴─────┴────┴────┴────┤
; │                              Segment Limit (0-15)                              │
; └────────────────────────────────────────────────────────────────────────────────┘

; Two overlapping segments, code and data, span the entire 4GB of addressable memory.
; Segmentation register configuration based on Intel's basic flat model.
gdt_start:

; GDT requires first 8 byte segment descriptor to be null.
null_desc:
  dd 0
  dd 0

; A readable code segment with kernel privilege level and page granularity.
code_desc:
  dw 0xFFFF             ; Segment limit 0-15
  dw 0                  ; Base address 0-15
  db 0                  ; Base address 16-23
  db 10011010b          ; P, Priv, S, Ex, DC, RW, Ac
  db 11001111b          ; Gr, Sz, L, reserved bit, segment limit 16-19
  db 0                  ; Base address 24-31

; A writable data segment with kernel privilege level and page granularity.
data_desc:
  dw 0xFFFF
  dw 0
  db 0
  db 10010010b
  db 11001111b
  db 0
gdt_end:                ; Allows assembler to calculate GDT size.

gdt_desc:
  dw gdt_end - gdt_start - 1 ; Size of GDT, one less than actual size.
  dd gdt_start               ; Address of GDT

CODE_SEG equ code_desc - gdt_start
DATA_SEG equ data_desc - gdt_start
