CC=gcc
CFLAGS=-Wall -g -lgmp

objects = main.o read.o numlist.o

acal: $(objects)
	$(CC) -o acal $(objects) $(CFLAGS)

main.o: main.c read.h

read.o: read.c read.h numlist.h numlist.c memwrap.h

numlist.o: numlist.c read.h numlist.h memwrap.h

.PHONY: clean
clean:
	rm acal $(objects)

