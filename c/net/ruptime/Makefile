
.PHONY: all clean
CC = gcc
CFLAGS = -g -Wall

all: ruptime ruptimed

ruptime: ruptime.c
	$(CC) $(CFLAGS) -o $@ $^

ruptimed: ruptimed.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o ruptime ruptimed

