/**
 * @file text.c
 * @author Mike Babb (mbabb),  Sanjana Cheerla (scheerl)
 * read and write from and to files
 */
#include "text.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief read the file specified by the file pointer
 * @param fp the file pointer
 */
void
readFile(FILE* fp)
{
    if (fp == NULL) {
        return;
    }

    int rowCount = 0;

    while (true) {
        if (rowCount > MAX_ROWS) {
            fprintf(stderr, "Too many lines\n");
            exit(1);
        }

        int colCount = 0;
        char c;

        if ((c = fgetc(fp)) == EOF) {
            break;
        } else {
            ungetc(c, fp);
        }

        while ((c = fgetc(fp)) != '\n') {
            if (colCount >= MAX_COLS - 1) {
                fprintf(stderr, "Line too long\n");
                exit(1);
            }
            FILE_STRING[rowCount][colCount] = c;
            colCount++;
        }
        rowCount++;
    }
    ROW_COUNT = rowCount;
}

/**
 * @brief write the file specified by the a file pointer
 * @param fp the file pointer
 */
void
writeFile(FILE* fp)
{
    for (int i = 0; i < ROW_COUNT; i++) {
        fprintf(fp, "%s\n", FILE_STRING[i]);
    }
}
