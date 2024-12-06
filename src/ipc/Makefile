CC=i686-elf-gcc
AR=i686-elf-ar

INCLUDE = -I../include/
OBJECT = ipc.o
TARGET = libipc.a

all: $(TARGET)

test: ipc_test

$(TARGET): $(OBJECT)
	$(AR) rcs $@ $^

%.o : %.c
	$(CC) -o $@ $^ $(INCLUDE) -c -ffreestanding -O2 -nostdlib -lgcc -g

ipc_test: test/ipc_test.c ipc.c
	gcc -o $@ $^ $(INCLUDE)

.PHONY: clean format

format:
	clang-format -i *.c

clean:
	$(RM) $(OBJECT) $(TARGET) ipc_test
