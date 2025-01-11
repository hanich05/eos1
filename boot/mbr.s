sector_1:
[org 0x7c00]
    mov [drive_num], dl

    ; tiny mode
    mov ax, 0x00
    mov ds, ax
    mov ss, ax
    mov es, ax

    ; call BIOS interrupt 0x13 (ah=0x02) - read sectors with a CHS address
    mov al, [sectors_count] ; sectors count
    dec al                  ; sector 1 was already read by the BIOS (this sector)
    mov ch, 0x00            ; cylinder
    mov cl, 0x02            ; sector
    mov dh, 0x00            ; head
    mov dl, [drive_num]     ; drive number
    ; mov es, 0x00          ; buffer segment - already set to 0x00
    mov bx, 0x7e00          ; buffer segment offset
    mov ah, 0x02
    int 0x13
    ; if a read error ocours, carry flag is set
    jc read_error
    ; sectores read are saved in al reg
    ; if (al != sectors_count-1) then not all sectors were read
    inc al
    cmp al, [sectors_count]
    jne read_error

    ; no read errors
    jmp sector_2

read_error:
    ; error ocoured. print error message
    mov bx, read_err_str
    mov ah, 0x0e
read_err_print_loop:
    mov al, [bx]
    int 0x10
    inc bx
    cmp byte [bx], 0x00
    jne read_err_print_loop
    
    ; infinite loop
    jmp $

drive_num:
    db 0x00
read_err_str:
    db "read error", 0x00

    ; padding
    times 0x1fd-($-sector_1) db 0x00
sectors_count:
    ; will be set after compilation to indicate how many sectors exist.
    ; byte 0x1fd (509) in the binary file.
    db 0x00

    ; magic master-boot-record bytes
    db 0x55, 0xaa

sector_2:
    mov bx, read_suc_str
    mov ah, 0x0e
read_suc_print_loop:
    mov al, [bx]
    int 0x10
    inc bx
    cmp byte [bx], 0x00
    jne read_suc_print_loop
    
    jmp $

read_suc_str:
    db "read success", 0x00
    times 0x200-($-sector_2) db 0x33
