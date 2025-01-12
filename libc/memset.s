[bits 32]

[global memset]

; System_V_ABI calling convention
; void* memset(void* s, int8_t c, int32_t n)
memset:
    push ebp
    mov ebp, esp
    push edi

    mov edi, [ebp+8] ; s
    mov ecx, [ebp+12] ; c
    mov edx, [ebp+16] ; n

    ; Preserve destination address because we have to return it.
    mov eax, edi

.loop:
    cmp edx, 0x00
    je .done
    mov byte [edi], cl
    inc edi
    dec edx
    jmp .loop

.done:

    pop edi
    mov esp, ebp
    pop ebp

    ret