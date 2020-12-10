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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * vector struct of data, size, capacity, and the size of the element
 */
typedef struct
{
    void* data;
    int size;
    int capacity;
    int element_size;
} Vector;

/**
 * @brief create vecor
 * @param element_size of the vector elements
 * @return created vector
 */
Vector
vector_create(int element_size);

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
vector_set(const Vector* vec, size_t ix, void* element);

/**
 * @brief get an element of the vector
 * @param ix to set the element of
 * @return thhe element at ix
 */
void*
vector_get(const Vector* vec, size_t ix);

/**
 * @brief add an element to the top of a vector
 * @param vec to add to
 * @param element to add
 * @return the vector pointer
 */
Vector*
vector_push(Vector* vec, void* element);

/**
 * @brief remove an element from the top of the vector
 * @param vec to pop an element off of
 */
void*
vector_pop(Vector* vec);

/**
 * @brief remove an element at ix
 * @param vec to remove from
 * @param ix to remove
 * @return removed element
 */
void*
vector_remove(Vector* vec, size_t ix);

#endif
