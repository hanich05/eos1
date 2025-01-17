CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
LD = /usr/local/i386elfgcc/bin/i386-elf-ld
DEBUG? = -g

os.bin: boot/boot.bin kernel/full_kernel.bin set_sectors_count_byte.bash pad_last_sector.bash
	cat "boot/boot.bin" "kernel/full_kernel.bin" > "os.bin"
	bash "./pad_last_sector.bash"
	bash "./set_sectors_count_byte.bash"

boot/boot.bin: boot/mbr.s
	nasm $(DEBUG?) $< -o $@ -f bin

kernel/full_kernel.bin: kernel/kernel_entry.o kernel/kernel.o drivers/pic/pic.o kernel/interrupts/isr.o kernel/interrupts/isr_stubs.o kernel/interrupts/idt.o kernel/text_mode.o libc/io.o libc/memmove.o libc/memset.o
	$(LD) -Ttext 0x1000 $^ --oformat binary -o $@

kernel/kernel_entry.o: kernel/kernel_entry.s kernel/gdt.s
	nasm $(DEBUG?) $< -f elf -o $@

kernel/kernel.o: kernel/kernel.c kernel/interrupts/isr.h kernel/text_mode.h
	$(CC) $(DEBUG?) -ffreestanding -m32 -g -c $< -o $@

drivers/pic/pic.o: drivers/pic/pic.c
	$(CC) $(DEBUG?) -ffreestanding -m32 -g -c $< -o $@

kernel/interrupts/isr.o: kernel/interrupts/isr.c kernel/gdt.h kernel/interrupts/idt.h drivers/pic/pic.h kernel/text_mode.h
	$(CC) $(DEBUG?) -ffreestanding -m32 -g -c $< -o $@

kernel/interrupts/isr_stubs.o: kernel/interrupts/isr_stubs.s
	nasm $(DEBUG?) $< -f elf -o $@

kernel/interrupts/idt.o: kernel/interrupts/idt.s
	nasm $(DEBUG?) $< -f elf -o $@

kernel/text_mode.o: kernel/text_mode.c libc/memmove.h libc/memset.h
	$(CC) $(DEBUG?) -ffreestanding -m32 -g -c $< -o $@

libc/io.o: libc/io.s
	nasm $(DEBUG?) $< -f elf -o $@

libc/memmove.o: libc/memmove.s
	nasm $(DEBUG?) $< -f elf -o $@

libc/memset.o: libc/memset.s
	nasm $(DEBUG?) $< -f elf -o $@

run: os.bin
	qemu-system-i386 os.bin

debug: os.bin
	qemu-system-i386 os.bin -s -S &
	gdb --init-eval-command="target remote localhost:1234" --eval-command="symbol-file os.bin"

clean: $(wildcard *.o */*.o */*/*.o)
	rm -f "boot/boot.bin" "kernel/full_kernel.bin" $^