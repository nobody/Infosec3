CC=cc
CFLAGS=
LIB= -lsocket -lnsl

all:	attack attack1

attack1:	attack1.c
	$(CC) $(CFLAGS) -o $@ $< $(LIB)

attack:	attack.c
	$(CC) $(CFLAGS) -o $@ $< $(LIB)

clean:
	rm -f attack attack1
