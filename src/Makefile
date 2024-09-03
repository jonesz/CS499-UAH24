CC=i686-elf-gcc
HDRS=term.h

kernel.bin: boot.o kernel.o term.o $(HDRS)
	$(CC) -T linker.ld -o $@ -ffreestanding -O2 -nostdlib $^ -lgcc

boot.o: boot.asm
	nasm -felf32 $^ -o $@

%.o: %.c
	$(CC) -c $< -std=gnu99 -ffreestanding -O2 -Wall -Wextra

.PHONY: clean format

clean:
	$(RM) *.o kernel.bin

format:
	clang-format -i *.c *.h

start:
	qemu-system-i386 -kernel kernel.bin

iso:
	install -D kernel.bin ../iso/boot/kernel.bin
	install -D ../grub.cfg ../iso/boot/grub/grub.cfg
	grub-mkrescue -o kernel.iso ../iso
	cp kernel.iso ../iso

start-iso:
	qemu-system-i386 -cdrom ../iso/kernel.iso