#
# Makefile for ordinary assignment 5 CSE 341 Spring 1999
#

CC=cc
CFLAGS=
LIB= -lsocket -lnsl

all:	listen speak listen1

listen1.o: listen1.c
	$(CC) $(CFLAGS) -c $<
listen.o: listen.c
	$(CC) $(CFLAGS) -c $<
speak.o: speak.c
	$(CC) $(CFLAGS) -c $<

listen1:	listen1.o
	$(CC) $(CFLAGS) -o $@ listen1.o $(LIB)

listen:	listen.o
	$(CC) $(CFLAGS) -o $@ listen.o $(LIB)

speak:	speak.o
	$(CC) $(CFLAGS) -o $@ speak.o $(LIB)

clean:
	rm -f *.o core listen speak listen
