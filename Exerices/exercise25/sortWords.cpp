#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Read words from cin into the given vector of strings.
void readWords( /* ... */ )
{
  // ...
}

// Use the index operator ( you know, [] ) to print out the list
// of words in reverse.
void printBackward( /* ... */ )
{
  cout << "-- Backward --" << endl;
  // ...
}

// Use iteratros to print the list forward.
void printForward( /* ... */ )
{
  cout << "-- Forward --" << endl;
  // ...
}

int main()
{
  // You can make a vector of strings.  Why not.
  vector< string > words;

  // Read a list of words.
  readWords( words );

  // Use a template algorithm to sort them.
  // ...

  // Print the list backward.
  printBackward( words );

  // Then print it again, forward this time.
  printForward( words );
}
