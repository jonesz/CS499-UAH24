CC=i686-elf-gcc
AR=i686-elf-ar

INCLUDE = -I../include/
OBJECT = test.o test_kmalloc.o
TARGET = libtest.a

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
