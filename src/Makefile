CC=i686-elf-gcc

ifdef RELEASE
	DEF="-D RELEASE"
endif

INCLUDE=include/

all: kernel.bin kernel.iso

kernel.bin: boot.o kernel.o mem/libmem.a interrupt/libinterrupt.a vid/libvid.a test/libtest.a sched/libsched.a syscalls/libsyscalls.a libk.a
	$(CC) -T linker.ld -o $@ -ffreestanding -O2 -nostdlib $^ -lgcc $(DEF) -I$(INCLUDE)

.PHONY: mem/libmem.a vid/libvid.a interrupt/libinterrupt.a test/libtest.a sched/libsched.a

syscalls/libsyscalls.a:
	+$(MAKE) -C syscalls

sched/libsched.a:
	+$(MAKE) -C sched

mem/libmem.a:
	+$(MAKE) -C mem

vid/libvid.a:
	+$(MAKE) -C vid
	
interrupt/libinterrupt.a:
	+$(MAKE) -C interrupt

test/libtest.a:
	+$(MAKE) -C test
	
libk.a:
	./build_libc.sh

%.o: %.c
	$(CC) -c -o $@ -ffreestanding -O2 -nostdlib $^ -lgcc $(DEF) -I$(INCLUDE)

%.o: %.asm
	nasm -felf32 $^ -o $@

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
	+$(MAKE) -C syscalls clean
	+$(MAKE) -C sched clean
	+$(MAKE) -C interrupt clean
	+$(MAKE) -C mem clean
	+$(MAKE) -C vid clean
	+$(MAKE) -C test clean
	$(RM) *.o *.a *.iso *.bin

format:
	+$(MAKE) -C sched clean
	+$(MAKE) -C interrupt format
	+$(MAKE) -C mem format
	+$(MAKE) -C vid format
	+$(MAKE) -C test format
