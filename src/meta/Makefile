CC=gcc

INCLUDE = -I../include/
SRCS = $(wildcard *.c)
TARGETS = $(SRCS:%.c=%)

all: metas

$(TARGETS): $(SRCS)
	$(CC) -o $@ $^ $(INCLUDE) -O2 -g
	mv $@ bin

.PHONY: metas clean format

metas: $(TARGETS)
	
	cd ..; meta/bin/$^

format:
	clang-format -i *.c

clean:
	$(RM) bin/$(TARGETS)
