
os.bin: boot.bin full_kernel.bin set_sectors_count_byte.bash pad_last_sector.bash
	cat "boot.bin" "full_kernel.bin" > "os.bin"
	bash ./pad_last_sector.bash
	bash ./set_sectors_count_byte.bash

boot.bin: boot/mbr.s boot/gdt.s
	nasm boot/mbr.s -o boot.bin -f bin

full_kernel.bin: kernel.o kernel_entry.o
	i386-elf-ld -Ttext 0x1000 "kernel_entry.o" "kernel.o" --oformat binary -o "full_kernel.bin"

kernel_entry.o: kernel/kernel_entry.s
	nasm "kernel/kernel_entry.s" -f elf -o "kernel_entry.o"

kernel.o: kernel/kernel.c
	i386-elf-gcc -ffreestanding -m32 -g -c "kernel/kernel.c" -o "kernel.o"

run: os.bin
	qemu-system-i386 os.bin
#	qemu-system-x86_64 boot.bin

clean:
	rm -f "boot.bin" "full_kernel.bin" "kernel_entry.o" "kernel.o"