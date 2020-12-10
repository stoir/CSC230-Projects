CC=gcc
CFLAGS=-Wall -std=c99 -g


hash: hash.o sha1.o buffer.o hmac-sha1.o
	$(CC) $(CFLAGS) hash.c sha1.c buffer.c hmac-sha1.c vector/vector.c -o hash


testdriver: testdriver.o sha1.o buffer.o
	$(CC) $(CFLAGS) -DTESTABLE testdriver.c sha1.c buffer.c vector/vector.c -o testdriver


testdriver2: testdriver2.o sha1.o buffer.o
	$(CC) $(CFLAGS) -DTESTABLE testdriver2.c sha1.c buffer.c vector/vector.c -o testdriver2

clean:
	rm -f *.o
	rm -rf *.dSYM/
	rm -f stdout.txt
	rm -f stderr.txt
	rm -f output.txt
	rm -f hash
	rm -f testdriver
	rm -f testdriver2
