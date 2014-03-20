
.PHONY: all clean
CC = gcc
CFLAGS = -g -Wall

all: test

test: test.c stack.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o test

