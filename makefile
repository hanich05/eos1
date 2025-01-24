CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
LD = /usr/local/i386elfgcc/bin/i386-elf-ld
DEBUG? = -g

os.bin: boot/boot.bin kernel/full_kernel.bin set_sectors_count_byte.bash pad_last_sector.bash
	cat "boot/boot.bin" "kernel/full_kernel.bin" > "os.bin"
	bash "./pad_last_sector.bash"
	bash "./set_sectors_count_byte.bash"

boot/boot.bin: boot/mbr.s
	nasm $(DEBUG?) $< -o $@ -f bin

kernel/full_kernel.bin: kernel/kernel_entry.o kernel/kernel.o drivers/ps2/ps2_keyboard/keys.o drivers/ps2/ps2_keyboard/ps2k_scancodes.o drivers/ps2/ps2_keyboard/ps2k.o drivers/ps2/ps2.o drivers/pic/pic.o kernel/interrupts/irqs/timer/timer.o kernel/interrupts/irqs/irqs.o kernel/interrupts/isrs.o kernel/interrupts/isr_stubs.o kernel/interrupts/idt.o kernel/text_mode.o libc/io.o libc/memmove.o libc/memset.o
	$(LD) -Ttext 0x1000 $^ --oformat binary -o $@

kernel/kernel_entry.o: kernel/kernel_entry.s kernel/gdt.s
	nasm $(DEBUG?) $< -f elf -o $@

kernel/kernel.o: kernel/kernel.c kernel/text_mode.h kernel/interrupts/isrs.h drivers/ps2/ps2.h drivers/ps2/ps2_keyboard/ps2k.h
	$(CC) $(DEBUG?) -ffreestanding -m32 -g -c $< -o $@

drivers/ps2/ps2_keyboard/keys.o: drivers/ps2/ps2_keyboard/keys.c drivers/ps2/ps2_keyboard/keys.h drivers/ps2/ps2_keyboard/keys.h kernel/text_mode.h
	$(CC) $(DEBUG?) -ffreestanding -m32 -g -c $< -o $@

drivers/ps2/ps2_keyboard/ps2k_scancodes.o: drivers/ps2/ps2_keyboard/ps2k_scancodes.c drivers/ps2/ps2_keyboard/ps2k_scancodes.h drivers/ps2/ps2_keyboard/keys.h
	$(CC) $(DEBUG?) -ffreestanding -m32 -g -c $< -o $@

drivers/ps2/ps2_keyboard/ps2k.o: drivers/ps2/ps2_keyboard/ps2k.c drivers/ps2/ps2_keyboard/ps2k.h drivers/ps2/ps2.h libc/memmove.h kernel/text_mode.h
	$(CC) $(DEBUG?) -ffreestanding -m32 -g -c $< -o $@

drivers/ps2/ps2.o: drivers/ps2/ps2.c drivers/ps2/ps2.h libc/io.h kernel/interrupts/irqs/timer/timer.h
	$(CC) $(DEBUG?) -ffreestanding -m32 -g -c $< -o $@

drivers/pic/pic.o: drivers/pic/pic.c
	$(CC) $(DEBUG?) -ffreestanding -m32 -g -c $< -o $@

kernel/interrupts/irqs/timer/timer.o: kernel/interrupts/irqs/timer/timer.c kernel/interrupts/irqs/timer/timer.h
	$(CC) $(DEBUG?) -ffreestanding -m32 -g -c $< -o $@

kernel/interrupts/irqs/irqs.o: kernel/interrupts/irqs/irqs.c kernel/interrupts/irqs/irqs.h libc/io.h drivers/pic/pic.h kernel/interrupts/irqs/timer/timer.h drivers/ps2/ps2.h kernel/text_mode.h
	$(CC) $(DEBUG?) -ffreestanding -m32 -g -c $< -o $@

kernel/interrupts/isrs.o: kernel/interrupts/isrs.c kernel/interrupts/isrs.h kernel/gdt.h kernel/interrupts/idt.h kernel/interrupts/irqs/irqs.h kernel/text_mode.h
	$(CC) $(DEBUG?) -ffreestanding -m32 -g -c $< -o $@

kernel/interrupts/isr_stubs.o: kernel/interrupts/isr_stubs.s
	nasm $(DEBUG?) $< -f elf -o $@

kernel/interrupts/idt.o: kernel/interrupts/idt.s
	nasm $(DEBUG?) $< -f elf -o $@

kernel/text_mode.o: kernel/text_mode.c kernel/text_mode.h libc/memmove.h libc/memset.h
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

clean: $(wildcard *.o */*.o */*/*.o */*/*/*.o)
	rm -f "os.bin" "boot/boot.bin" "kernel/full_kernel.bin" $^