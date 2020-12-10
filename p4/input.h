/**
 * @file input.h
 * @author Mike Babb
 * @author Sanjana Cheerla
 * @brief read a line of input and returns it
 */
#ifndef input_h
#define input_h

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAP 10

/**
 * @brief read in a line of data from the file pointer
 * @param fp to read data from
 * @return what was read
 */
char*
readLine(FILE* fp);

#endif /* input_h */
