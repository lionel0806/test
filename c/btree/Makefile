.PHONY: all clean
CC = gcc
CFLAGS = -g -Wall

all: btree

btree: btree.c btree.h
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o btree

