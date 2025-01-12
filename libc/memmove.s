[bits 32]

[global memmove]

; System_V_ABI calling convention
; void* memmove(void* dest, const void* src, int32_t n)
memmove:
    push ebp
    mov ebp, esp
    push edi
    push esi

    mov edi, [ebp+8] ; dest
    mov esi, [ebp+12] ; src
    mov edx, [ebp+16] ; n

    ; Preserve destination address because we have to return it.
    mov eax, edi

    ; If dest < src, we can always do a fast pointer-incrementing move.
    ; If dest == src, do nothing.
    cmp edi, esi
    je .done
    jb .fast

    ; If dest > src and there are no overlapping regions (dest >= src+num), we
    ; can still do a fast pointer-incrementing move.
    mov ecx, esi
    add ecx, edx
    cmp edi, ecx
    jae .fast

; If dest > src and dest < src+num, we have to do a right-to-left move to
; preserve overlapping data.
.slow:

    ; Set the direction flag so copying is right-to-left.
    std

    ; Set the move count register.
    mov ecx, edx

    ; Update pointers to the right-hand side (minus one).
    dec edx
    add esi, edx
    add edi, edx

    ; Do a byte-by-byte move.
    rep movsb

    ; Reset the direction flag.
    cld

    jmp .done

.fast:

    ; Do a byte-by-byte move.
    mov ecx, edx
    rep movsb

.done:

    pop esi
    pop edi
    mov esp, ebp
    pop ebp

    ret