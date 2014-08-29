
.PHONY: all clean
CC = gcc
CFLAGS = -g -Wall

all: udpcli udpsvr udpsvr1

udpcli: udpcli.c
	$(CC) $(CFLAGS) -o $@ $^

udpsvr: udpsvr.c
	$(CC) $(CFLAGS) -o $@ $^
	
udpsvr1: udpsvr1.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o udpcli udpsvr udpsvr1

