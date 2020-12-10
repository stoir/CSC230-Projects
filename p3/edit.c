/**
 * @file edit.c
 * @author Mike Babb (mbabb),  Sanjana Cheerla (scheerl)
 * edit the data to remove or edit specific lines and colums
 */
#include "edit.h"

#include "text.h"

#include <stdio.h>
#include <string.h>

/**
 * file string for file data
 */
extern char FILE_STRING[MAX_ROWS][MAX_COLS];

/**
 * row count to track rows
 */
extern int ROW_COUNT;

/**
 * clamp the value x inbetween min and max
 * @param x to check for clamp
 * @param min value of clamp
 * @param max value of clamp
 * @return the value between min and max inclusive
 */
static int
clamp(const int x, const int min, const int max)
{
    if (x < min) {
        return min;
    } else if (x > max) {
        return max;
    } else {
        return x;
    }
}

/**
 * @brief swap a and b
 * @param a to swap to b
 * @param b to swap to a
 */
static void
swap_char(char* a, char* b)
{
    char t = *a;
    *a = *b;
    *b = t;
}

/**
 * @brief remove a row
 * @param row to remove
 */
static void
remove_row(int row)
{
    row = clamp(row, 0, MAX_ROWS);

    for (int i = row; i < MAX_ROWS - 1; i++) {
        strcpy(FILE_STRING[i], FILE_STRING[i + 1]);
    }

    ROW_COUNT -= 1;
}

/**
 * @brief remove a col
 * @param col to remove
 */
static void
remove_col(int row, int col)
{
    col = clamp(col, 0, MAX_COLS);

    for (int j = col; j < MAX_COLS - 1; j++) {
        swap_char(&FILE_STRING[row][j], &FILE_STRING[row][j + 1]);
    }
    FILE_STRING[row][MAX_COLS - 1] = '\0';
}

static void
remove_cols(int col)
{
    for (int i = 0; i < MAX_ROWS; i++) {
        remove_col(i, col);
    }
}

/**
 * @brief remove lines from start to end exclusive
 * @param start to start removing lines from (inclusive)
 * @param end to stop removing lines from (exclusive)
 */
void
removeLines(int start, int end)
{
    start = clamp(start, 1, ROW_COUNT) - 1;
    end = clamp(end, 1, ROW_COUNT) - 1;

    int row = start;
    while (start != end + 1) {
        remove_row(row);
        end -= 1;
    }
}

/**
 * @brief remove cols from start to end exclusive
 * @param start to start removing cols from (inclusive)
 * @param end to stop removing cols from (exclusive)
 */
void
removeCols(int start, int end)
{
    start -= 1;
    end -= 1;

    int col = start;
    while (start != end + 1) {
        remove_cols(col);
        end -= 1;
    }
}

/**
 * @brief edit just one line
 * @param lno line number to edit
 * @param start where the line should be edited from
 * @param end where the line should end
 */
void
editLine(int row, int start, int end)
{
    row -= 1;
    start = clamp(start - 1, 0, ROW_COUNT);
    end = clamp(end - 1, 0, ROW_COUNT);

    int col = start;
    while (start != end + 1) {
        remove_col(row, col);
        end -= 1;
    }
}
