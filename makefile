.PHONY: clean all
CC=gcc
CFLAGS=-I.


all : mutex modfac deadlock

mutex : 
	$(CC) mutex.c -lpthread -o mutex $(CFLAGS)

modfac :
	$(CC) modfac.c -lpthread -o modfac $(CFLAGS)

deadlock : 
	$(CC) deadlock.c -lpthread -o deadlock $(CFLAGS)

clean :
	rm mutex modfac deadlock
