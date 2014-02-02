CFLAGS=-Wall -g -lgmp

all: acal.c numlist.c read.c
	gcc $(CFLAGS) acal.c read.c numlist.c -o acal
clean:
	rm -f acal

