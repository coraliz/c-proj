
all: assembler

INCLUDE_DIR := include
SRCS := $(wildcard *.c)
OBJS := $(SRCS:.c=.o)

.PHONY: assembler clean

assembler: $(OBJS)
	gcc -g -Wall -ansi -pedantic $^ -lm -o $@

%.c: %.h

%.o: %.c
	gcc -c -g -Wall -ansi -pedantic -I$(INCLUDE_DIR) $< -o $@

clean:
	rm -rf *.o *.exe *.ent *.ext *.ob assembler