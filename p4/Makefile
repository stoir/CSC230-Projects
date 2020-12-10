CC=gcc
CFLAGS=-Wall -std=c99 -g
LIST=reading.o catalog.o input.o vector.o


reading: reading.o catalog.o input.o
	$(CC) $(CFLAGS) reading.c catalog.c input.c vector/vector.c -o reading


clean:
	rm -f *.o
	rm -f chop
	rm -rf *.dSYM/
	rm -f stdout.txt
	rm -f stderr.txt
	rm -f output.txt
	rm -f reading
