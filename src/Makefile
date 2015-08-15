CFLAGS=-Wall -g -lgmp -O2

objects = main.o \
	  tmpvars.o \
	  memwrap.o \
	  strbuf.o \
	  read.o \
	  numlist.o \
	  numstack.o \
	  globals.o \
	  arith.o

acal: $(objects)
	$(CC) -o acal $(CFLAGS) $(objects)

main.o: main.c read.h globals.h tmpvars.h exit_status.h

tmpvars.o: tmpvars.c tmpvars.h

memwrap.o: memwrap.c memwrap.h

strbuf.o: strbuf.c strbuf.h memwrap.h

read.o: read.c read.h numlist.h globals.h memwrap.h \
	strbuf.h exit_status.h arith.h

numlist.o: numlist.c numlist.h memwrap.h tmpvars.h

numstack.o: numstack.c numstack.h numlist.h memwrap.h

globals.o: globals.c globals.h

arith.o: arith.c arith.h exit_status.h numlist.h numstack.h

.PHONY: clean
clean:
	rm acal $(objects)

