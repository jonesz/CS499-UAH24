CC=i686-elf-gcc
AR=i686-elf-ar

INCLUDE = -I../include/
OBJECT = interrupt.o isr_wrappers.o key_handler.o
TARGET = libinterrupt.a

all: $(TARGET)

$(TARGET): $(OBJECT)
	$(AR) rcs $@ $^

%.o : %.c
	$(CC) -o $@ $^ $(INCLUDE) -c -ffreestanding -O2 -nostdlib -lgcc -g

%.o: %.asm
	nasm -felf32 $^ -o $@

.PHONY: clean format

format:
	clang-format -i *.c

clean:
	$(RM) $(OBJECT) $(TARGET)
