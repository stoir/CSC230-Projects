#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void readGrid( int rows, int cols, int grid[ rows ][ cols ] )
{
  // Add code to read in all the elements of grid from standard input.
  // ...
}

// Add parameters to to pass a variable-sized array to the following
// function.  It's the same as the previous function, but I want you
// to get a chance to type it in yourself.
void reportMaxima( /* ... */ )
{
  // Add code to find local maxima and print them out in row major order.
  // ...
}

int main()
{
  // Add code to read in the grid size from standard input.
  // ...

  // Declare a variable-sized array to hold the grid.
  // ...

  // Call readGrid() to populate the array.
  // ...
  
  // Call reportMaxima() to print out all local maxima.
  // ...
  
  return EXIT_SUCCESS;
}
