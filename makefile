
boot.bin: boot/mbr.s
	nasm boot/mbr.s -o boot.bin -f bin

run: boot.bin
	qemu-system-i386 boot.bin
#	qemu-system-x86_64 boot.bin