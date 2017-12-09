CFLAGS = -ffreestanding -O2 -g -Wall -Wextra -lgcc -fno-exceptions -fno-rtti -Ikernel/include -Ikernel/kernel
CC = i686-elf-g++
ASSEMBLER = i686-elf-as

OBJECTS = bin/kernel/stdafx.o bin/kernel/terminal.o bin/kernel/kernel.o bin/boot.o bin/kernel/keyboard.o bin/kernel/Port.o

default:
	make kernel
	make bootstrap
	make link

bootstrap:
	$(ASSEMBLER) boot.s -o bin/boot.o

kernel:

	$(CC) -c kernel/kernel/terminal.cpp -o bin/kernel/terminal.o $(CFLAGS)
	$(CC) -c kernel/kernel/kernel.cpp -o bin/kernel/kernel.o $(CFLAGS)
	$(CC) -c kernel/kernel/stdafx.cpp -o bin/kernel/stdafx.o $(CFLAGS)
	$(CC) -c kernel/kernel/keyboard.cpp -o bin/kernel/keyboard.o $(CFLAGS)

	$(ASSEMBLER) kernel/Port.S -o bin/kernel/Port.o

link:
	
	$(CC) -T linker.ld -o bin/kernel.bin -ffreestanding -O2 -nostdlib -Wall $(OBJECTS) -lgcc

clean:
	rm bin/kernel/terminal.o
	rm bin/kernel/kernel.o
	rm bin/boot.o
