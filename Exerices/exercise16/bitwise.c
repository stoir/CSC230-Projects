#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

/*
 * Clear the 6 high-order bits of the given short value, return the result.
 */
unsigned short clear6( unsigned short v )
{
  // ...
}

/*
 * Set the low-order 9 bits of the given short value, passed by address.
 */
void set9( unsigned short *v )
{
  // ...
}

/*
 * Set just bit 7 (counting from low-order bit zero) to 1 if flag is true.
 * clear it otherwise.
 */
void set7( unsigned short *v, bool flag )
{
  // ...
}

int main()
{
  // Try out our functions.  Don't change any of the lines below.  My test program may
  // automatically modify this part of your source code when it tests your implementation.

  printf( "----- clear6 -----\n" );

  printf( "%04x\n", clear6( 0xFFFF ) );
  printf( "%04x\n", clear6( 0x0000 ) );
  printf( "%04x\n", clear6( 0xA37D ) );
  printf( "%04x\n", clear6( 0x469E ) );

  printf( "----- set9 -----\n" );

  // For the next two functions, we need a real variable.
  unsigned short v;
  
  v = 0xFFFF;
  set9( &v );
  printf( "%04x\n", v );

  v = 0x0000;
  set9( &v );
  printf( "%04x\n", v );

  v = 0xB842;
  set9( &v );
  printf( "%04x\n", v );

  v = 0x19C0;
  set9( &v );
  printf( "%04x\n", v );

  printf( "----- set7 -----\n" );

  v = 0xFFFF;
  set7( &v, 0 );
  printf( "%04x\n", v );

  v = 0x0000;
  set7( &v, 1 );
  printf( "%04x\n", v );

  v = 0x5A70;
  set7( &v, 1 );
  printf( "%04x\n", v );

  v = 0x4F96;
  set7( &v, 1 );
  printf( "%04x\n", v );

  return 0;
}
