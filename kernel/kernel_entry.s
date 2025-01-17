[bits 16]
; the master boot record will jump to this position
kernel_entry:
    ; switch to protected mode
    ; load_gdt
    cli
    lgdt [gdt_descriptor]
    ; change bit 0 of cr0 to 1 - this bit indicates wether or not the system is in protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    ; change bit 11 of cr4 to 0 - this bit indicates the privilege of the current code (0 being the highest privelege)
    mov eax, cr4
    and eax, 0xff7f
    mov cr4, eax
    ; update cs register by preforming a far jump
    jmp CODE_SEG:protected_mode_start

%include "kernel/gdt.s"

[bits 32]
protected_mode_start:
    ; set up segments and stack for kernel
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x90000
    mov esp, ebp

    ; go to kernel main
[extern kmain]
    call kmain
    ; halt program if kmain returns for any reason
    jmp $
