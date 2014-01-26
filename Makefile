CFLAGS=-Wall -g -lgmp

all: acal.c
	gcc $(CFLAGS) acal.c -o acal
clean:
	rm -f acal

