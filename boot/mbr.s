sector_1:
[org 0x7c00]
    mov [drive_num], dl

    mov ax, 0x00
    mov es, ax          ; buffer segment
    mov bx, 0x7e00      ; buffer segment offset
    mov ah, 0x02        ; reading chs address
    mov al, [sectors_count] ; sector count
    mov ch, 0x00        ; cylinder
    mov cl, 0x02        ; sector
    mov dh, 0x00        ; head
    mov dl, [drive_num] ; drive number
    int 0x13
    
    jnc sector_2
    
    mov bx, read_err_str
    mov ah, 0x0e
read_err_print_loop:
    mov al, [bx]
    int 0x10
    inc bx
    cmp byte [bx], 0x00
    jne read_err_print_loop
    
    jmp $

drive_num:
    db 0x00
read_err_str:
    db "read error", 0x00

sectors_count:
    db ((end_of_sectors-$$) / 0x200 - 1)

    ; magic master-boot-record bytes
    times 0x1fe-($-sector_1) db 0x00
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

end_of_sectors: ; used to calculate length of .bin file in bytes