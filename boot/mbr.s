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

    ; save results
    mov [read_return_code], ah
    mov [read_actual_sectors_read_count], al

    ; if a read error ocours, carry flag is set
    jc read_error
    ; sectors read amount is saved in al reg
    ; if (al != sectors_count-1) then not all sectors were read
    inc al
    cmp al, [sectors_count]
    jne read_error

    ; no read errors
    ; call BIOS interrupt 0x10 ah=0x0e - print char
    mov ah, 0x0e
    mov bx, read_suc_str
print_read_suc_str_loop:
    mov al, [bx]
    int 0x10
    inc bx
    cmp byte [bx], 0x00
    jne print_read_suc_str_loop
    
    ; jmp to the kernel setup code (kernel_entry)
    jmp KERNEL_LOCATION

read_error:
    ; call BIOS interrupt 0x10 ah=0x0e - print char
    mov ah, 0x0e
    mov bx, read_err_str
print_read_err_str_loop:
    mov al, [bx]
    int 0x10
    inc bx
    cmp byte [bx], 0x00
    jne print_read_err_str_loop

    mov bh, [read_return_code]
    call print_hex8

    ; print (' ')
    mov al, 0x20
    mov ah, 0x0e
    int 0x10
    
    mov bh, [read_actual_sectors_read_count]
    call print_hex8

    ; infinite loop
    jmp $

; expects the value to be printed to be in bh
print_hex8:
    mov ah, 0x0e
    mov bl, 0x02
print_hex8_loop:
    mov al, bh
    and al, 0xf0
    shr al, 0x04
    cmp al, 0x09
    jg print_hex8_print_ab
print_hex8_print_01:
    add al, 0x30 ; += '0'
    jmp print_hex8_print_char
print_hex8_print_ab:
    sub al, 0x0a ; -= 10
    add al, 0x41 ; += 'A'
print_hex8_print_char:
    int 0x10
    ;
    shl bh, 0x04
    dec bl
    cmp bl, 0x00
    jne print_hex8_loop
    ret


read_return_code:
    db 0x00
read_actual_sectors_read_count:
    db 0x00
read_err_str:
    db "BIOS hard disk read error ", 0x00
read_suc_str:
    db "BIOS hard disk read success", 0x00
drive_num:
    db 0x00

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