CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
LD = /usr/local/i386elfgcc/bin/i386-elf-ld

os.bin: boot/boot.bin kernel/full_kernel.bin set_sectors_count_byte.bash pad_last_sector.bash
	cat "boot/boot.bin" "kernel/full_kernel.bin" > "os.bin"
	bash "./pad_last_sector.bash"
	bash "./set_sectors_count_byte.bash"

boot/boot.bin: boot/mbr.s boot/gdt.s
	nasm "boot/mbr.s" -o "boot/boot.bin" -f bin

kernel/full_kernel.bin: kernel/kernel.o kernel/kernel_entry.o kernel/text_mode.o libc/memmove.o libc/memset.o
	$(LD) -Ttext 0x1000 "kernel/kernel_entry.o" "kernel/kernel.o" "kernel/text_mode.o" "libc/memmove.o" "libc/memset.o" --oformat binary -o "kernel/full_kernel.bin"

kernel/kernel_entry.o: kernel/kernel_entry.s
	nasm "kernel/kernel_entry.s" -f elf -o "kernel/kernel_entry.o"

kernel/kernel.o: kernel/kernel.c kernel/text_mode.h
	$(CC) -ffreestanding -m32 -g -c "kernel/kernel.c" -o "kernel/kernel.o"

kernel/text_mode.o: kernel/text_mode.c libc/memmove.h libc/memmove.h
	$(CC) -ffreestanding -m32 -g -c "kernel/text_mode.c" -o "kernel/text_mode.o"

libc/memmove.o: libc/memmove.s
	nasm "libc/memmove.s" -f elf -o "libc/memmove.o"
	
libc/memset.o: libc/memset.s
	nasm "libc/memset.s" -f elf -o "libc/memset.o"

run: os.bin
	qemu-system-i386 os.bin
#	qemu-system-x86_64 boot.bin

clean:
	rm -f "boot/boot.bin" "kernel/full_kernel.bin" "kernel/kernel_entry.o" "kernel/kernel.o" "kernel/text_mode.o" "libc/memmove.o" "libc/memset.o"