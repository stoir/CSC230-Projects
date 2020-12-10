/**
 * @file input.h
 * @author Mike Babb
 * @author Sanjana Cheerla
 * @brief read a line of input and returns it
 */
#include "input.h"

/**
 * capacity increment multiplier
 */
#define CAP_INCR 2

/**
 * @brief read in a line of data from the file pointer
 * @param fp to read data from
 * @return what was read
 */
char*
readLine(FILE* fp)
{
    int i = 0;
    int capacity = CAP;

    char* buff = (char*) malloc(capacity * sizeof(char) + 1);

    while (true) {
        char ch = fgetc(fp);

        if (i >= capacity - 1) {
            capacity *= CAP_INCR;
            char* t_buff = (char*) realloc(buff, capacity * sizeof(char) + 1);

            if (t_buff == NULL) {
                free(buff);
                exit(1);
            } else {
                buff = t_buff;
            }
        }

        if (ch == '\n' || ch == EOF) {
            break;
        } else {
            buff[i] = ch;
        }

        i += 1;
    }

    if (i > 0) {
        buff[i] = '\0';
        return buff;
    } else {
        free(buff);
        return NULL;
    }
}
