//
// A program for students to debug.  It gets stuck in an infinite loop when you run it.
//
// You could figure out what's going wrong with the program by reading
// the source code, but that's not the point.  The point is to get some
// practice using the debugger.
//

#include <stdio.h>
#include <stdlib.h>

int f( int x ) {
  int r = 0;

  for ( int i = 0; i <= x; i++ )
    r += i;

  return r;
}

int g( int a, int b ) {
  int i = 0;

  while ( a < b ) {
    a++;
    i++;
  }

  return i;
}


int h( int x ) {
  int j = 0;
  for ( int i = 0; i <= 10; i += x + 1 )
    j += i;

  return j;
}

int x( int a, int b ) {
  int i = 0;

  while ( a < b ) {
    h( a );
    a++;
    i++;
  }

  return i;
}

int main( void )
{
  f( 10 );
  g( 20, 30 );
  x( 10, 20 );
  g( 20, 30 );
  f( 9 );
  x( -1, 10 );
  f( 30 );

  return EXIT_SUCCESS;
}
