/**
 * @file vector.c
 * @author Mike Babb
 * @author Sanjana Cheerla
 * @brief maintain a vector of objects for
 * creating destroying and modifying the vector
 */
#include "vector.h"

/**
 * @brief create vecor
 * @param element_size of the vector elements
 * @param initial_capacity of vector
 * @return created vector
 */
Vector
vector_create(size_t element_size, size_t initial_capacity)
{
    const size_t size = 0;

    Vector vec = {.data = NULL,
                  .size = size,
                  .capacity = initial_capacity,
                  .element_size = element_size};
    vec.data = calloc(initial_capacity, sizeof(VECTOR_TYPE) * element_size);

    return vec;
}

/**
 * @brief destroy vecor
 * @param vec to destroy
 */
void
vector_destroy(Vector* vec)
{
    free(vec->data);
    vec = NULL;
}

/**
 * @brief grows vector
 * @param vec to grow
 * @retuurn growed vector
 */
Vector*
vector_grow(Vector* vec)
{
    if (vec->size >= vec->capacity - 1) {
        vec->capacity *= 2;
    } else {
        return vec;
    }

    VECTOR_TYPE new_data =
      realloc(vec->data,
              sizeof(VECTOR_TYPE) * vec->element_size * vec->capacity);

    if (new_data == NULL) {
        free(vec->data);
        return NULL;
    } else {
        vec->data = new_data;
        return vec;
    }
}

/**
 * @brief get an element of the vector
 * @param ix to set the element of
 * @return the element at ix
 */
VECTOR_TYPE
vector_get(const Vector* vec, size_t ix)
{
    return vec->data + (vec->element_size * ix);
}

/**
 * @brief change an element of the vector
 * @param vec to get the element of
 * @param ix to set the element of
 * @param element new element
 */
void
vector_set(const Vector* vec, size_t ix, VECTOR_TYPE element)
{
    memcpy(vector_get(vec, ix), element, vec->element_size);
}

/**
 * @brief add an element to the top of a vector
 * @param vec to add to
 * @param element to add
 * @return the vector pointer
 */
Vector*
vector_push(Vector* vec, VECTOR_TYPE element)
{
    vec = vector_grow(vec);
    memcpy(vector_get(vec, vec->size), element, vec->element_size);
    vec->size += 1;

    return vec;
}

/**
 * @brief remove an element from the top of the vector
 * @param vec to pop an element off of
 */
VECTOR_TYPE
vector_pop(Vector* vec)
{
    VECTOR_TYPE element = vector_get(vec, vec->size - 1);
    vec->size -= 1;

    return element;
}
