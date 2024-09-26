CC=i686-elf-gcc
HDRS=term.h

ifdef RELEASE
	DEF="-D RELEASE"
endif

all: kernel.bin kernel.iso

kernel.bin: kmalloc.o paging.o gdt.o boot.o kernel.o term.o test.o test_kmalloc.o $(HDRS) libk.a
	$(CC) -T linker.ld -o $@ -ffreestanding -O2 -nostdlib $^ -lgcc $(DEF)
	
libk.a:
	./build_libc.sh

boot.o: boot.asm
	nasm -felf32 $^ -o $@

%.o: test/%.c
	$(CC) -c $< -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I include/ $(DEF)

%.o: %.c
	$(CC) -c $< -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I include/ $(DEF)

kernel.iso: kernel.bin grub.cfg
	mkdir -p build/
	install -D kernel.bin build/boot/kernel.bin
	install -D grub.cfg build/boot/grub/grub.cfg
	grub-mkrescue -o kernel.iso build/
	$(RM) -r build/

start: kernel.bin
	qemu-system-i386 -kernel kernel.bin

start-iso: kernel.iso
	qemu-system-i386 -cdrom kernel.iso

.PHONY: clean format

clean:
	$(RM) *.o kernel.bin kernel.iso *.a

format:
	clang-format -i *.c *.h
