CFLAGS=-Wall -g -lgmp

all: acal.c numlist.c
	gcc $(CFLAGS) acal.c numlist.c -o acal
clean:
	rm -f acal

