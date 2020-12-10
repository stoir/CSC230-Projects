//NEW Can you do more ambitious arithmetic? Sort strings instead?  Do something with every nth element? Process from end to start instead? Find the median element?
//NEW 
//
//
//
// A (partial) implementation of selection sort,
// using lots of pointer arithmetic.

#include <stdio.h>

// Swap the values pointed to by a and b.
void swap( int *a, int *b )
{
  // Implement this funciton, easy
  // ...
}

// Return a pointer to the element of the given array with the smallest value.
int *findSmallest( int *list, int len )
{
  int *smallest = list;
//NEW - no indexing!
  for ( int i = 1; i < len; i++ ) {
    // Fill in the body of this loop, without
    // declaring any more variables.  Just use
    // the smallest pointer to keep up with the
    // smallest value you've found so far.
    // ...
  }

  return smallest;
}

// Print the contents of the given list.
//NEW - no indexing!
void printList( int *list, int len )
{
  while ( len ) {
    // Fill in the body of this loop, without
    // declaring any more variables.  Modify the parameters as you
    // iterate, to move through the list and exit the loop when you've
    // printed everything.
    // ...
  }

  printf( "\n" );
}

int main()
{
  // A list of random-ish values.
  int list[] = { 808, 250, 74, 659, 931, 273, 545, 879, 924, 710, 441, 166 };
//NEW - read from standard input instead
  int len = sizeof( list ) / sizeof( list[ 0 ] );

  // For each index, find the smallest item from the remaining
  // (unsorted) portion of the list.
  for ( int i = 0; i < len; i++ ) {

  //NEW - redo, truee pointer arithmetic, no indexing

    // Here's a clever trick.  Call findSmallest() with a pointer to
    // the start of the unsorted part of list.  It's just getting an offset
    // into our list array, but it can use as if it was a whole (shorter) array.
//NEW - redo how variables passed
    int *p = findSmallest( list + i, len - i );

    // Swap the smallest item into the first position in the unsorted part of the
    // list.
//NEW - redo how variables passed
    swap( list + i, p );

    // Now, the sorted part of the list is a little bit longer and the
    // unsorted part is a little shorter.
  }

  printList( list, len );
}
