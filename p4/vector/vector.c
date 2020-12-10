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
 * @return created vector
 */
Vector
vector_create(int element_size)
{
    const int initial_capacity = 5;
    const int size = 0;

    Vector vec = {.data = NULL,
                  .size = size,
                  .capacity = initial_capacity,
                  .element_size = element_size};

    vec.data = calloc(initial_capacity, sizeof(void*) * element_size);

    if (vec.data == NULL) {
        printf("Vector create failed\n");
        exit(1);
    }

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

    void* new_data =
      realloc(vec->data, sizeof(void*) * vec->element_size * vec->capacity);

    if (new_data == NULL) {
        printf("Realloc failed.");
        free(vec->data);
        exit(1);
        return NULL;
    } else {
        vec->data = new_data;
        return vec;
    }
}

/**
 * @brief swaps val1 with val2
 * @param val1 to swap
 * @param val2 to swap
 * @param len of object
 */
void
swap(void* val1, void* val2, size_t len)
{
    unsigned char* p = val1;
    unsigned char* q = val2;
    unsigned char tmp;

    for (size_t i = 0; i < len; i++) {
        tmp = p[i];
        p[i] = q[i];
        q[i] = tmp;
    }
}

/**
 * @brief remove an element at ix
 * @param vec to remove from
 * @param ix to remove
 * @return removed element
 */
void*
vector_remove(Vector* vec, size_t ix)
{
    for (int i = ix; i < vec->size - 1; i++) {
        swap(vector_get(vec, i), vector_get(vec, i + 1), vec->element_size);
    }
    return vector_pop(vec);
}

/**
 * @brief get an element of the vector
 * @param ix to set the element of
 * @return thhe element at ix
 */
void*
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
vector_set(const Vector* vec, size_t ix, void* element)
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
vector_push(Vector* vec, void* element)
{
    vec = vector_grow(vec);
    vector_set(vec, vec->size, element);
    vec->size += 1;
    return vec;
}

/**
 * @brief remove an element from the top of the vector
 * @param vec to pop an element off of
 */
void*
vector_pop(Vector* vec)
{
    void* element = vector_get(vec, vec->size - 1);
    vec->size -= 1;

    return element;
}
