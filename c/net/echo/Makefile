
.PHONY: all clean
CC = gcc
CFLAGS = -g -Wall

all: echosvr echocli

echosvr: echosvr.c common.c common.h
	$(CC) $(CFLAGS) -o $@ $^

echocli: echocli.c common.c common.h
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o echosvr echocli

