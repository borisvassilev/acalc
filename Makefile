CC=gcc
CFLAGS=-Wall -g -lgmp

objects = main.o read.o numlist.o numstack.o

acal: $(objects)
	$(CC) -o acal $(CFLAGS) $(objects)

main.o: main.c read.h

read.o: read.c read.h numlist.h numlist.c

numlist.o: numlist.c numlist.h memwrap.h

stack.o: numstack.c numstack.h numlist.h numlist.c memwrap.h

.PHONY: clean
clean:
	rm acal $(objects)

