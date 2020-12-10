CC=gcc
CFLAGS=-Wall -std=c99 -g


omni-q: omni-q.o list.o data.o vector/vector.o
	$(CC) $(CFLAGS) omni-q.c list.c data.c vector/vector.c -o omni-q


clean:
	rm -f *.o
	rm -rf *.dSYM/
	rm -f stdout.txt
	rm -f stderr.txt
	rm -f output.txt
	rm -f omni-q
	rm -f vector/*.o
