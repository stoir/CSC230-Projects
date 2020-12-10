#ifndef edit_h
#define edit_h
/**
 * @file edit.h
 * @author Mike Babb (mbabb),  Sanjana Cheerla (scheerl)
 * edit the data to remove or edit specific lines and colums
 */
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief remove lines from start to end exclusive
 * @param start to start removing lines from (inclusive)
 * @param end to stop removing lines from (exclusive)
 */
void
removeLines(int start, int end);

/**
 * @brief edit just one line
 * @param lno line number to edit
 * @param start where the line should be edited from
 * @param end where the line should end
 */
void
editLine(int lno, int start, int end);

/**
 * @brief remove cols from start to end exclusive
 * @param start to start removing cols from (inclusive)
 * @param end to stop removing cols from (exclusive)
 */
void
removeCols(int start, int end);

#endif
