INCLUDE = -I../include/
OBJECT = term.o
TARGET = libvid.a

all: $(TARGET)

$(TARGET): $(OBJECT)
	$(AR) rcs $@ $^

%.o : %.c
	$(CC) -o $@ $^ $(INCLUDE) -c

.PHONY: clean format

format:
	clang-format -i *.c

clean:
	$(RM) $(OBJECT) $(TARGET)
