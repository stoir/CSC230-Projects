/**
 * @file vector.h
 * @author Mike Babb
 * @author Sanjana Cheerla
 * @brief maintain a vector of objects for
 * creating destroying and modifying the vector
 */
#ifndef vector_h
#define vector_h

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VECTOR_TYPE void*

/** vector struct */
typedef struct
{
    VECTOR_TYPE data;
    size_t size;
    size_t capacity;
    size_t element_size;
} Vector;

/**
 * @brief create vecor
 * @param element_size of the vector elements
 * @param initial_capacity of vector
 * @return created vector
 */
Vector
vector_create(size_t element_size, size_t initial_capacity);

/**
 * @brief destroy vecor
 * @param vec to destroy
 */
void
vector_destroy(Vector* vec);

/**
 * @brief change an element of the vector
 * @param vec to get the element of
 * @param ix to set the element of
 * @param element new element
 */
void
vector_set(const Vector* vec, size_t ix, VECTOR_TYPE element);

/**
 * @brief get an element of the vector
 * @param ix to set the element of
 * @return the element at ix
 */
VECTOR_TYPE
vector_get(const Vector* vec, size_t ix);

/**
 * @brief add an element to the top of a vector
 * @param vec to add to
 * @param element to add
 * @return the vector pointer
 */
Vector*
vector_push(Vector* vec, VECTOR_TYPE element);

/**
 * @brief remove an element from the top of the vector
 * @param vec to pop an element off of
 */
VECTOR_TYPE
vector_pop(Vector* vec);

#endif
