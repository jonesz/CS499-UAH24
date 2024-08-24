BUILD_DIR=build

kernel.bin: $(BUILD_DIR)/boot.o $(BUILD_DIR)/kernel.o
	i686-elf-gcc -T linker.ld -o $@ -ffreestanding -O2 -nostdlib $^ -lgcc

$(BUILD_DIR)/boot.o: boot.asm
	nasm -felf32 $^ -o $@

$(BUILD_DIR)/kernel.o: kernel.c
	i686-elf-gcc -c $^ -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra

.PHONY: clean format

clean:
	$(RM) $(BUILD_DIR)/*

format:
	clang-format -i *.c *.h
