[bits 16]
; global descriptor table
; this is a flat-model gdt. meaning base=0 and limit=max
[global gdt]
align 0x10
gdt:
    ; entry 0 - always null
    dd 0x00000000, 0x00000000
    
CODE_SEG equ 0x08 ; this value is also hard-set in gdt.h
    ; entry 1 - code segment descriptor
    ; limit bits 15-0
    dw 0xffff
    ; base bits 15-0
    dw 0x0000
    ; base bits 23-16
    db 0x00
    ; access byte
    ;   bit 7    = Present (P)                      | 1
    ;   bits 6-5 = Descriptor privilege level (DPL) | 00
    ;   bit 4    = Descriptor type (S)              | 1
    ;   bit 3    = Executable (E)                   | 1
    ;   bit 2    = Direction/Conforming (DC)        | 0
    ;   bit 1    = Readable/Writable (RW)           | 1
    ;   bit 0    = Accessed (A)                     | 0
    db 0b10011010
    ; flags bits 3-0
    ;   flag bit 3 = Granularity (G) | 1
    ;   flag bit 2 = Size flag (DB)  | 1
    ;   flag bit 1 = Long mode (L)   | 0
    ;   flag bit 0 = reserved        | 0
    ; limit bits 19-16               | 1111
    db 0b11001111
    ; base bits 31-24
    db 0x00

DATA_SEG equ 0x10 ; this value is also hard-set in gdt.h
    ; entry 2 - data segment descriptor
    ; limit bits 15-0
    dw 0xffff
    ; base bits 15-0
    dw 0x0000
    ; base bits 23-16
    db 0x00
    ; access byte
    ;   bit 7    = Present (P)                      | 1
    ;   bits 6-5 = Descriptor privilege level (DPL) | 00
    ;   bit 4    = Descriptor type (S)              | 1
    ;   bit 3    = Executable (E)                   | 0
    ;   bit 2    = Direction/Conforming (DC)        | 0
    ;   bit 1    = Readable/Writable (RW)           | 1
    ;   bit 0    = Accessed (A)                     | 0
    db 0b10010010
    ; flags bits 3-0
    ;   flag bit 3 = Granularity (G) | 1
    ;   flag bit 2 = Size flag (DB)  | 1
    ;   flag bit 1 = Long mode (L)   | 0
    ;   flag bit 0 = reserved        | 0
    ; limit bits 19-16               | 1111
    db 0b11001111
    ; base bits 31-24
    db 0x00
gdt_end:

gdt_descriptor:
    ; size
    dw (gdt_end - gdt -1)
    ; offset
    dd gdt