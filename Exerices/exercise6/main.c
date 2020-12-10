#include <stdio.h>
#include <stdlib.h>
#include "print.h"

int main()
{
  int ch;
  
  // Read all the characters from standard input.
  while ( ( ch = getchar() ) != EOF ) {
    // Let the print component print out this character.
    dashesForSpaces( ch );
  }

  // Report the number of spaces, as counted by the print component.
  printf( "\n" );
  printf( "That contained %d spaces\n", spaceCount );

  return EXIT_SUCCESS;
}
