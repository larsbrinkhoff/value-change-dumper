CFLAGS += -Wall -O2

.PHONY: all clean

all: test.vcd

vcd.o: vcd.c vcd.h

check.o: check.c vcd.h

check: vcd.o check.o
	$(CC) -o $@ $^

test.vcd: check check.vcd
	./$<
	cmp $@ check.vcd || rm -f $@

clean:
	-rm -f *.o check test.vcd
