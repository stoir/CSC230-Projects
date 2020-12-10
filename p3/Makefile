CC=gcc
CFLAGS=-Wall -std=c99 -g

chop: chop.o text.o lexer.o parser.o edit.o vector.o
	$(CC) $(CFLAGS) chop.c text.c lexer.c parser.c edit.c vector.c -o chop

clean:
	rm -f *.o
	rm -f chop
	rm -rf *.dSYM/
	rm -f stdout.txt
	rm -f stderr.txt
	rm -f output.txt
	rm -f __tmp.txt
