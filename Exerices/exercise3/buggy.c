//
// A program for students to debug.  It crashes when you run it.
// You could figure out what's going wrong with the program by reading
// the source code, but that's not the point.  The point is to get some
// practice using the debugger.
//

#include <stdio.h>
#include <stdlib.h>

int vince( int x ) {
  int r = 0;

  for ( int i = 0; i <= x; i++ )
    r += i;

  return r;
}

int isabel( int a, int b ) {
  int i = 0;

  while ( a < b ) {
    a++;
    i++;
  }

  return i;
}


int herbert( int x ) {
  int j = 0;
  for ( int i = x; i <= 10; i += 2 )
    j++;

  return j;
}

int ethel( int a, int b ) {
  int i = 0;

  while ( a < b ) {
    herbert( a );
    a++;
    i += 100 % a;
  }

  return i;
}

void wanda( int a, int b ) {
  ethel( b, a );
}

int main( void )
{
  vince( 10 );
  isabel( 20, 30 );
  ethel( 10, 20 );
  isabel( 20, 30 );
  vince( 9 );
  wanda( 10, -10 );
  vince( 30 );

  return EXIT_SUCCESS;
}
