define sizeof
	$$(stat -L -c %s $(1))
endef


boot.bin: boot/mbr.s set_sectors_count_byte.bash
	nasm boot/mbr.s -o boot.bin -f bin
	bash ./set_sectors_count_byte.bash

run: boot.bin
	qemu-system-i386 boot.bin
#	qemu-system-x86_64 boot.bin