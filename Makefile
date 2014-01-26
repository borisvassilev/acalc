CFLAGS=-Wall -g

all: acal.c
	gcc acal.c -lgmp -o acal
clean:
	rm -f acal

