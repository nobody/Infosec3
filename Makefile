CC = cc -g

PROGS = exploit1 exploit2

all: $(PROGS)

exploit2: exploit2.c
	$(CC) -o $@ $<

exploit1: exploit1.c
	$(CC) -o $@ $<


clean: 
	rm -f exploit1 exploit2
