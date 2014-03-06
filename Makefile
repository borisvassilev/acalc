CC=gcc
CFLAGS=-Wall -g -lgmp

objects = main.o \
	  tmpvars.o \
	  memwrap.o \
	  strbuf.o \
	  read.o \
	  numlist.o \
	  numstack.o \
	  state.o

acal: $(objects)
	$(CC) -o acal $(CFLAGS) $(objects)

main.o: main.c read.h state.h tmpvars.h exit_status.h

tmpvars.o: tmpvars.c tmpvars.h

memwrap.o: memwrap.c memwrap.h

strbuf.o: strbuf.c strbuf.h memwrap.h

read.o: read.c read.h numlist.h state.h memwrap.h strbuf.h exit_status.h

numlist.o: numlist.c numlist.h memwrap.h tmpvars.h

numstack.o: numstack.c numstack.h numlist.h memwrap.h

state.o: state.c state.h

.PHONY: clean
clean:
	rm acal $(objects)

