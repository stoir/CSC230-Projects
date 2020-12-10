CC=gcc
CFLAGS=-Wall -std=c99 -g

blur: blur.o lexer.o parser.o image.o
	$(CC) $(CFLAGS) -o blur blur.o lexer.o parser.o image.o

brighten: brighten.o lexer.o parser.o image.o
	$(CC) $(CFLAGS) -o brighten brighten.o lexer.o parser.o image.o

border: border.o lexer.o parser.o image.o
	$(CC) $(CFLAGS) -o border border.o lexer.o parser.o image.o
	
clean:
	rm -f blur.o brighten.o border.o lexer.o parser.o image.o
	rm -f blur brighten border