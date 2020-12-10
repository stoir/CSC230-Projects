#ifndef text_h
#define text_h

/**
 * @file text.h
 * @author Mike Babb (mbabb),  Sanjana Cheerla (scheerl)
 * header file to read and write from and to files
 */
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * max rows of a file
 */
#define MAX_ROWS 10000

/**
 * max cols of a file
 */
#define MAX_COLS 101

/**
 * file string of data
 */
char FILE_STRING[MAX_ROWS][MAX_COLS];

/**
 * row count to keep track of rows
 */
int ROW_COUNT;

/**
 * @brief read the file specified by the file pointer
 * @param fp the file pointer
 */
void
readFile(FILE* fp);

/**
 * @brief write the file specified by the a file pointer
 * @param fp the file pointer
 */
void
writeFile(FILE* fp);

#endif
