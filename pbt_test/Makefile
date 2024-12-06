CC = aarch64-linux-gnu-gcc

CFLAGS	= 
LDFLAGS = -static

default: pbttest


ledtest: pbttest.o
	$(CC) $(LDFLAGS) -o pbttest pbttest.o

ledtest.o: pbttest.c
	$(CC) $(CFLAGS) -o pbttest.o -c pbttest.c

clean:
	rm pbttest.o pbttest
