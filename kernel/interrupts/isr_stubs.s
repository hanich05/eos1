[bits 32]

[global isr_stubs_table]
isr_stubs_table:
%assign i 0
%rep 255
    dd isr_stub%+i
%assign i (i+1)
%endrep

%macro isr_no_error_stub 1
isr_stub%+%1:
    push 0x00   ; push dummy error code
    push %1     ; push interrupt number
    jmp isr_common
%endmacro
%macro isr_error_stub 1
isr_stub%+%1:
                ; error code pushed by cpu
    push %1     ; push interrupt number
    jmp isr_common
%endmacro

isr_no_error_stub 0
isr_no_error_stub 1
isr_no_error_stub 2
isr_no_error_stub 3
isr_no_error_stub 4
isr_no_error_stub 5
isr_no_error_stub 6
isr_no_error_stub 7
isr_error_stub 8
isr_no_error_stub 9
isr_error_stub 10
isr_error_stub 11
isr_error_stub 12
isr_error_stub 13
isr_error_stub 14
isr_no_error_stub 15
isr_no_error_stub 16
isr_error_stub 17
isr_no_error_stub 18
isr_no_error_stub 19
isr_no_error_stub 20
isr_error_stub 21
; 22-255 are no error interupts
%assign i 22
%rep (256-21)
isr_no_error_stub i
%assign i (i+1)
%endrep


[extern interrupt_handler]
; System_V_ABI calling convention
; void interrupt_handler(struct registers_t* regs)

isr_common:
    ; push edi, esi, ebp, esp, ebx, edx, ecx, eax
    pushad
    
    ; push ds
    xor eax, eax
    mov ax, ds
    push eax

    ; use kernel data segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; System-V-ABI calling convention requires DF to be clear on function entry
    cld
    ; pass pointer to stack (struct registers_t) as argument
    push esp
    ; call function
    call interrupt_handler
    ; remove argument from stack
    add esp, 4

    ; restore ds
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; pop edi, esi, ebp, esp, ebx, edx, ecx, eax
    popad
    ; remove interrupt number and error code from stack
    add esp, 8
    ; return from interrupt
    iret