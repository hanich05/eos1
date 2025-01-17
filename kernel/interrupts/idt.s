[bits 32]

[global idt]
align 0x10
idt:
    times (256*8) db 0x00

[global idt_descriptor]
align 0x4
idt_descriptor:
    dw (256*8)-1
    dd idt

[global idt_set_gate]
; system-V-ABI calling convention
; void idt_set_gate(uint8_t interrupt, uint16_t segment_selector, void (*base)(void), uint8_t flags)
idt_set_gate:
    push ebp
    mov ebp, esp

    ; eax = &(idt[interrupt])
    xor eax, eax
    mov byte al, [ebp+8]
    lea eax, [idt+eax*8]

    ; ecx = base
    mov ecx, [ebp+16]
    ; set low base bits
    mov word [eax], cx
    ; set high base bits
    shr ecx, 16
    mov word [eax+6], cx
    
    ; ecx = segment_selector
    mov ecx, [ebp+12]
    ; set segment selector bits
    mov word [eax+2], cx

    ; ecx = flags
    xor ecx, ecx
    mov ch, [ebp+20]
    ; set flag bits (and reserved bits to 0x00)
    and cx, 0xff00
    mov word [eax+4], cx

    ; dont return anything
    xor eax, eax

    mov esp, ebp
    pop ebp
    ret

[global idt_load]
; system-V-ABI calling convention
; void idt_load()
idt_load:
    push ebp
    mov ebp, esp

    lidt [idt_descriptor]

    mov esp, ebp
    pop ebp
    ret