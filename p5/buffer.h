/**
 * @file buffer.h
 * @author Mike Babb
 * @author Sanjana Cheerla
 * @brief Buffer
 */
#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "vector/vector.h"

#include <stdint.h>

/** Representation for the contents of an input file, copied to memory. */
typedef struct {
    size_t cap;
    size_t len;
    unsigned char* data;
    Vector data_vector;
} Buffer;

/**
 * @brief creates a buffer
 * @return buffer
 */
Buffer*
makeBuffer(void);

/**
 * @brief appends byte to buffer
 * @param buff to append
 * @param byte to append to buff
 */
void
appendBuffer(Buffer* buff, unsigned char byte);

/**
 * @brief free buffer
 * @param buff to free
 */
void
freeBuffer(Buffer* buff);

/**
 * @brief read the string into a buffer
 * @param s to read
 * @return buffer
 */
Buffer*
read_str(char* s);

/**
 * @brief read the file into a buffer
 * @param filename to read
 * @return buffer
 */
Buffer*
readFile(const char* filename);

#endif
