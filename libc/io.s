[bits 32]

[global outb]
; System_V_ABI calling convention
; void outb(uint16_t port, uint8_t byte)
outb:
    push ebp
    mov ebp, esp

    mov dx, [ebp+8]

    mov al, [ebp+12]
    out dx, al

    mov esp, ebp
    pop ebp
    ret

[global inb]
; System_V_ABI calling convention
; uint8_t inb(uint16_t port)
inb:
    push ebp
    mov ebp, esp

    mov dx, [ebp+8]

    xor eax, eax
    in al, dx

    mov esp, ebp
    pop ebp
    ret
