CC=i686-elf-gcc
AR=i686-elf-ar

INCLUDE = -I../include/
OBJECT = gdt.o paging.o kmalloc.o buddy_alloc.o fixed_alloc.o bump_alloc.o
TARGET = libmem.a

all: $(TARGET)

$(TARGET): $(OBJECT)
	$(AR) rcs $@ $^

%.o : %.c
	$(CC) -o $@ $^ $(INCLUDE) -c -ffreestanding -O2 -nostdlib -lgcc -g

.PHONY: clean format

format:
	clang-format -i *.c

clean:
	$(RM) $(OBJECT) $(TARGET)
