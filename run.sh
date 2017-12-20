if grub-file --is-x86-multiboot bin/kernel.bin; then
	echo Multiboot Confirmed, launching kernel via qemu
	qemu-system-i386 -kernel bin/kernel.bin -s -device isa-debug-exit,iobase=0xf4,iosize=0x04 -monitor stdio
else
	echo The file does not multiboot
fi
