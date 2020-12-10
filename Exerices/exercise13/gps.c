#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/** Multiplier for converting degrees to radians */
#define DEG_TO_RAD ( M_PI / 180 )

/** Radius of the earth in miles. */
#define EARTH_RADIUS 3959.0

/** Print a usage message to standard error and exit unsuccessfully. */
static void usage()
{
  fprintf( stderr, "usage: gps <city-file>\n" );
  exit( EXIT_FAILURE );
}

/** General-purpose function to complain about the input format (which
    shouldn't actualy happen when we're testing your code (but it's good
    to think about how you could handle this). */
static void invalidInput()
{
  fprintf( stderr, "invalid input\n" );
  exit( EXIT_FAILURE );
}

/** Given the latitude and longitude of two locations, return the (approximate)
    distance between them, in miles. */
double globalDistance( double lat1, double lon1, double lat2, double lon2 )
{
  // OK, pretend the center of the earth is at the origin, turn these
  // two locations into vectors pointing from the origin.
  // This could be simplified.
  double v1[] = { cos( lon1 * DEG_TO_RAD ) * cos( lat1 * DEG_TO_RAD ),
                  sin( lon1 * DEG_TO_RAD ) * cos( lat1 * DEG_TO_RAD ),
                  sin( lat1 * DEG_TO_RAD ) };

  double v2[] = { cos( lon2 * DEG_TO_RAD ) * cos( lat2 * DEG_TO_RAD ),
                  sin( lon2 * DEG_TO_RAD ) * cos( lat2 * DEG_TO_RAD ),
                  sin( lat2 * DEG_TO_RAD ) };

  // Dot product these two vectors.
  double dp = 0.0;
  for ( int i = 0; i < sizeof( v1 ) / sizeof( v1[ 0 ] ); i++ )
    dp += v1[ i ] * v2[ i ];
  
  // Compute the angle between the vectors based on the dot product.
  double angle = acos( dp );

  // Return distance based on the radius of the earth.
  return EARTH_RADIUS * angle;
}

/** Struct representation for a city, with a name and a location in latitude
    and longitude. */

// ...

/** Read the name and location of a city from the given file, and store it
    in the struct pointed to by cty. */
void readCity( FILE *fp, struct City *cty ) {

  // ...

}

int main( int argc, char *argv[] )
{
  if ( argc != 2 )
    usage();

  // Open an input file given as the only command-line argument.
  FILE *fp = fopen( argv[ 1 ], "r" );
  if ( !fp ) {
    fprintf( stderr, "Can't open file: %s\n", argv[ 1 ] );
    usage();
  }

  // Read the number of cities
  int n;
  if ( fscanf( fp, "%d", &n ) != 1 || n < 1 )
    invalidInput();

  // Dynamically allocate an array of City structs that's large enough for
  // all the cities we have to read.
  
  // ...

  // Use the readCity function to fill in the fields of all the cities.

  // ...

  // Should be done with the input file now.
  fclose( fp );


  // Keep reading query locations until we get something that's not a pair
  // of doubles.
  double lat, lon;
  while ( scanf( "%lf%lf", &lat, &lon ) == 2 ) {
    // Find and report the nearest city.

    // ...
   
  }

  // Free the memory for the list of cities.

  // ...

  return EXIT_SUCCESS;
}
