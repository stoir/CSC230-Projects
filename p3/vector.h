#ifndef vector_h
#define vector_h

/**
 * @file vector.h
 * @author Mike Babb (mbabb),  Sanjana Cheerla (scheerl)
 * stores the command line arguments into a vector
 */
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * vector consisting of arguments, size, capacity, and the size of the elements
 */
typedef struct
{
    void** data;
    int size;
    int capacity;
    int element_size;

} Vector;

/**
 * @brief create a vector
 * @param element size to create vector from
 * @return vector created
 */
Vector
vector_create(int element_size);

/**
 * @brief destroy a vector
 * @param vec to destroy
 */
void
vector_destroy(Vector* vec);

void*
vector_get(const Vector* vec, size_t idx);

/**
 * @brief push an argument onto a vector
 * @param vec to push arg to
 * @param arg to push onto vector
 * @return new pointer to the pushed argument
 */
Vector*
vector_push(Vector* vec, void* arg);

/**
 * @brief pop an element off a vector
 * @param vec to pop off an argument from
 * @return pointer to argument that was popped off
 */
void*
vector_pop(Vector* vec);

#endif
