KERNEL_LOCATION equ 0x1000

sector_1:
[org 0x7c00]

    mov [drive_num], dl

    ; tiny model
    mov ax, 0x00
    mov es, ax
    mov ds, ax
    mov ss, ax
    mov bp, 0x8000
    mov sp, bp

    ; call BIOS interrupt 0x10 ah=0x00 - switch to text mode (clears screen)
    mov ah, 0x00
    mov al, 0x03
    int 0x10

    ; call BIOS interrupt 0x10 ah=0x01 - set cursor shape
    mov cx, 0x2607 ; invisible cursor
    mov ah, 0x01
    int 0x10

    ; call BIOS interrupt 0x13 ah=0x02 - read sectors with a CHS address
    mov al, [sectors_count] ; sectors count
    dec al                  ; sector 1 was already read by the BIOS (this sector)
    mov ch, 0x00            ; cylinder
    mov cl, 0x02            ; sector
    mov dh, 0x00            ; head
    mov dl, [drive_num]     ; drive number
    ; mov es, 0x00          ; buffer segment - already set to 0x00
    mov bx, KERNEL_LOCATION ; buffer segment offset (read to KERNEL_LOCATION)
    mov ah, 0x02
    int 0x13

    ; if a read error ocours, carry flag is set
    jc read_error
    ; sectors read amount is saved in al reg
    ; if (al != sectors_count-1) then not all sectors were read
    inc al
    cmp al, [sectors_count]
    jne read_error

    ; no read errors
    ; set read_res
    mov byte [read_res], 0x01
    ; print success message
    mov bx, read_suc_str
    jmp print_read_res_str

read_error:
    ; error ocoured. print error message
    mov bx, read_err_str

print_read_res_str:
    ; call BIOS interrupt 0x10 ah=0x0e - print char
    mov ah, 0x0e
print_read_res_str_loop:
    mov al, [bx]
    int 0x10
    inc bx
    cmp byte [bx], 0x00
    jne print_read_res_str_loop
    
    ; jump if read succedded
    cmp byte [read_res], 0x00
    jne read_success

    ; infinite loop
    jmp $

read_success:
    
    ; switch to protected mode
    ; load_gdt
    cli
    lgdt [gdt_descriptor]
    ; change last bit of cr0 to 1
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    ; update cs register by preforming a far jump
    jmp CODE_SEG:protected_mode_start

read_res:
    db 0x00 ; will be set to 1 if read succeded
read_err_str:
    db "BIOS hard disk read error", 0x00
read_suc_str:
    db "BIOS hard disk read success", 0x00
drive_num:
    db 0x00
    
%include "boot/gdt.s"

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
    ; go to kernel (kernel_entry)
    jmp KERNEL_LOCATION


[bits 16]
    ; padding
    times 0x1fd-($-sector_1) db 0x00
sectors_count:
    ; will be set after compilation to indicate how many sectors exist.
    ; byte 0x1fd (509) in the binary file.
    db 0x00

    ; magic master-boot-record bytes
    ; bytes 0x1fe and 0x1ff
    db 0x55, 0xaa



; ... kernel goes here ...