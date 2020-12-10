#include <stdio.h>
#include <stdlib.h>

int main()
{
  // Check command-line arguments and open the input file.
  FILE *fp;
  if ( ( fp = fopen( "input.txt", "r" ) ) == NULL ) {
    fprintf( stderr, "Can't open file: input.txt\n" );
    exit( EXIT_FAILURE );
  }

  // Allocate a string with a small, initial capacity.
  int capacity = 5;
  char *buffer = /* ... add initial call to malloc ... */ ;

  // Number of characters we've ready so far
  int len = 0;
  
  // Read from this file until we reach end-of-file, storing all characters
  // in buffer, and keeping track of the number of characters reead.
  // Keep resizing the buffer as needed until it contains the
  // contents of the whole file.  This took me 9 lines of code.
  // ...

  // After reading the entire file, make sure the contents of buffer
  // are null-terminated

  // Print out the total size of the file, in characters.
  printf( "%d\n", len );

  // Print out the whole file; it's just one big string.
  printf( "%s", buffer );

  // Don't leak memory or leave open files.
  // ...

  return 0;
}
