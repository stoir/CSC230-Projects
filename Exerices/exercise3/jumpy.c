//
// A program for playing with the debugger.
//
// The program is a little silly, but it gives us something we can
// trace with the debugger.  We have a small collection of functions
// that generate similar-looking output. The program randomly selects
// one and calls it.  Your job is to trace the program's execution and
// figure out which funciton printed each message.  With help from the
// debugger, we can tell which function is responsible for each line
// of ourput.
//
// (this would be a good job for function pointers, but we haven't
// learned about those yet).
//

#include <stdio.h>
#include <stdlib.h>

void edith( int x )
{
  // Edith likes numbers that are even.
  if ( x % 2 == 0 )
    printf( "I like %d\n", x );
  else
    printf( "I don't like %d\n", x );
}

void tyler( int x )
{
  // Tyler likes numbers that are less than 40.
  if ( x < 40 )
    printf( "I like %d\n", x );
  else
    printf( "I don't like %d\n", x );
}


void melonie( int x )
{
  // Melonie likes number that have a 2 or a 7 in them.

  // Write out the number as a string.
  char buffer[ 20 ];
  sprintf( buffer, "%d", x );

  // Look for an occurrence of a 2 or a 7.
  for ( int i = 0; buffer[ i ]; i++ )
    if ( buffer[ i ] == '2' || buffer[ i ] == '7' ) {
      printf( "I like %d\n", x );
      return;
    }

  // If we didn't find one of those digits, we don't like the number.
  printf( "I don't like %d\n", x );
}

void aaron( int x )
{
  // Aaron likes number that are larger than
  // the last one he saw.

  static int previous = -1;

  if ( x > previous )
    printf( "I like %d\n", x );
  else
    printf( "I don't like %d\n", x );
  
  previous = x;
}


int main( void )
{
  // Here's where we call the randomly selected functions.
  for ( int i = 0; i < 5; i++ ) {
    // Make up a random number.
    int number = rand() % 100;

    // Give that number to a randomly selected function.
    int choice = rand() % 4;
    switch ( choice ) {
    case 0:
      edith( number );
      break;
    case 1:
      tyler( number );
      break;
    case 2:
      melonie( number );
      break;
    case 3:
      aaron( number );
      break;
    }
  }

  return EXIT_SUCCESS;
}
