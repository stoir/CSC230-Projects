// A simple program that uses structured looping statements to report all the
// prime numbers in a given range.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
  Return true if the given value is prime.
 */
bool isPrime( unsigned int val )
{
  // Check all smaller values as potential factors.
  for ( unsigned int fact = 2; fact * fact <= val; fact++ )
    // Does this factor evenly divide the given value.
    if ( val % fact == 0 ) {
      // if so, the value isn't prime.
      return false;
    }

  // If the value survives all the tests for factors, it must be prime.
  return true;
}

/*
  Starting point for this program.  Test a sequence of values for
  prime-ness.
 */
int main( void )
{
  // Get the range of values we're supposed to test.
  unsigned int low, high;
  if ( scanf( "%d%d", &low, &high ) != 2 ) {
    printf( "Invalid input\n" );
    exit( EXIT_FAILURE );
  }

  // Test all values from low up to and including high.
  for ( unsigned int v = low; v <= high; v++ ) {
    // If this value is prime, print it out.
    if ( isPrime( v ) ) {
      printf( "%u\n", v );
    }
  }
  
  return EXIT_SUCCESS;
}
