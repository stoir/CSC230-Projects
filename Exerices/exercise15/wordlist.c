#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/** Maximum length of a words.  Words longer than this are
    truncated. */
#define WORD_MAX 8

/** Node, used to store a linked list of words. */
typedef struct NodeStruct {
  /** Next node on the linked list. */
  struct NodeStruct *next;
  
  /** Copy of the word read fromt he file. */
  char word[ WORD_MAX ];
} Node;

/** Print a usage message, then exit unsuccessfully. */
static void usage()
{
  fprintf( stderr, "usage: words <input-file>\n" );
  exit( EXIT_FAILURE );
}

/** Return true if the given character could be part of a word, either
 a letter or an apostrophe. */
bool wordChar( int ch )
{
  return ( ch >= 'a' && ch <= 'z' ) ||
    ( ch >= 'A' && ch <= 'Z' ) ||
    ch == '\'';
}

/** Return the lower-case version of the given code.  There's a standard
    library function for this, but we haven't seen it yet.*/
int toLower( int ch )
{
  if ( ch >= 'A' && ch <= 'Z' )
    return ch - 'A' + 'a';
  return ch;
}

/** Read all the words from the given file, an return them as a
    linked list. */
Node *readWords( char *filename )
{
  // Open the given file.
  FILE *fp = fopen( filename, "r" );
  if ( !fp ) {
    fprintf( stderr, "Can't open file: %s\n", filename );
    usage();
  }

  // Linked list of all the words we find in the file.
  Node *list = NULL;

  while ( true ) {
    // Skip non-word characters.
    int ch = fgetc( fp );
    while ( ch != EOF && ! wordChar( ch ) )
      ch = fgetc( fp );

    // Make a node to store the next word.
    Node *n = (Node *) malloc( sizeof( Node ) );
    
    // Read all the letters and store them as a string in the node.
    int len = 0;
    while ( wordChar( ch ) ) {
      // Discard characters after we reach the capacity of a word.
      if ( len < WORD_MAX )
        n->word[ len++ ] = toLower( ch );
      ch = fgetc( fp );
    }
    
    // Mark the end of this string.
    n->word[ len ] = '\0';

    // The most recent word could only be empty if we reached the
    // end-of-file.  If so, return the list.
    if ( len == 0 ) {
      return list;
    }

    // Link this node into the list.
    n->next = list;
    list = n;
  }
}


/** Insertion sort on a linked list. */
Node *sortList( Node *list )
{
  Node *newList = NULL;

  // Move nodes from list, to newList, putting them in sorted order.
  while ( list ) {
    // Remove the next node from list.
    Node *n = list;
    list = list->next;
    
    // Find the link to the ndoe that n needs to go before.
    Node **target = &newList;
    while ( *target && strcmp( n->word, (*target)->word ) >= 0 )
      target = &(*target)->next;

    // Insert node n before node we just found.
    n->next = *target;
    *target = n;
  }
  
  return newList;
}

/** Given a sorted list, report the number of occurrences of each word. */
void reportWords( Node *list )
{
  Node *n = list;
  while ( n ) {
    // Find the first node that's a different word, counting how many
    // copies of the same word we pass along the way.
    Node *end = n->next;
    int ocount;
    while ( end && strcmp( n->word, end->word ) == 0 ) {
      end = end->next;
      ocount++;
    }

    // Print this word, along with its number of occurrences.
    printf( "%s (%d)\n", n->word, ocount );

    // Move on to the next word.
    n = end;
  }
}

int main( int argc, char *argv[] )
{
  if ( argc != 2 )
    usage();

  // Read al the words from the file given on the command line.
  Node *list = readWords( argv[ 1 ] );

  // Sort all the words.
  list = sortList( list );

  // Report the number of occurrences of each word.
  reportWords( list );

  // Free the list of nodes.
  while ( list ) {
    free( list );
    list = list->next;
  }

  return EXIT_SUCCESS;
}
