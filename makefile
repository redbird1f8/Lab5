.PHONY: clean all
CC=gcc
CFLAGS=-I.


all : mutex modfac

mutex : 
	$(CC) mutex.c -lpthread -o mutex $(CFLAGS)

modfac :
	$(CC) modfac.c -lpthread -o modfac $(CFLAGS)

clean :
	rm mutex modfac
