
.PHONY: all clean
CC = gcc
CFLAGS = -g -Wall

all: test

test: test.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o test

