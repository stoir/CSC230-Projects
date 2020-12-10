/**
 * @file buffer.c
 * @author Mike Babb
 * @author Sanjana Cheerla
 * @brief Buffer
 */

#include "buffer.h"

#include "vector/vector.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFF_SZ 1024
#define SIXTY_FOUR_BIT 64

/**
 * @brief aligns a vector data to buffer data
 * @param buff to align
 */
void
align_buffer_vector(Buffer* buff)
{
    buff->cap = buff->data_vector.capacity;
    buff->len = buff->data_vector.size;
    buff->data = buff->data_vector.data;
}

/**
 * @brief creates a buffer
 * @return buffer
 */
Buffer*
makeBuffer()
{
    Buffer* buff = (Buffer*) malloc(sizeof(Buffer));
    buff->data_vector = vector_create(sizeof(unsigned char), SIXTY_FOUR_BIT);

    align_buffer_vector(buff);

    return buff;
}

/**
 * @brief appends byte to buffer
 * @param buff to append
 * @param byte to append to buff
 */
void
appendBuffer(Buffer* buff, unsigned char byte)
{
    vector_push(&buff->data_vector, &byte);
    align_buffer_vector(buff);
}

/**
 * @brief free buffer
 * @param buff to free
 */
void
freeBuffer(Buffer* buff)
{
    vector_destroy(&buff->data_vector);
    free(buff);
}

/**
 * @brief read the string into a buffer
 * @param s to read
 * @return buffer
 */
Buffer*
read_str(char* s)
{
    Buffer* buff = makeBuffer();

    for (int i = 0; i < strlen(s); i++) {
        appendBuffer(buff, s[i]);
    }

    return buff;
}

/**
 * @brief read the file into a buffer
 * @param filename to read
 * @return buffer
 */
Buffer*
readFile(const char* filename)
{
    FILE* fp = fopen(filename, "rb");

    const int buff_size = BUFF_SZ;
    unsigned char t_buff[buff_size];
    size_t bytes_read = 0;

    if (fp != NULL) {
        Buffer* buff = makeBuffer();

        while (true) {
            bytes_read = fread(t_buff, 1, sizeof(t_buff), fp);

            if (bytes_read <= 0) {
                break;
            } else {
                for (int i = 0; i < bytes_read; i++) {
                    appendBuffer(buff, t_buff[i]);
                }
            }
        }
        fclose(fp);
        return buff;
    } else {
        return NULL;
    }
}
