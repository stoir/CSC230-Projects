/**
 * @file vector.c
 * @author Mike Babb (mbabb),  Sanjana Cheerla (scheerl)
 * stores the command line arguments into a vector
 */
#include "vector.h"

/**
 * @brief create a vector
 * @param element size to create vector from
 * @return vector created
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
    vec.data = malloc(sizeof(void*) * element_size * initial_capacity);
    return vec;
}

/**
 * @brief destroy a vector
 * @param vec to destroy
 */
void
vector_destroy(Vector* vec)
{
    free(vec->data);
    vec = NULL;
}

/**
 * @brief shrink or grow the vector
 * @param vec to shrink or grow
 * @param shrink if the vector should be shrinked or not
 * @return the pointer to the new adjusted vector
 */
Vector*
vector_grow(Vector* vec)
{
    if (vec->size >= vec->capacity) {
        vec->capacity *= 2;
    } else {
        return vec;
    }

    void** new_data =
      realloc(vec->data, sizeof(void*) * vec->element_size * vec->capacity);

    if (new_data == NULL) {
        free(vec->data);
        return NULL;
    } else {
        vec->data = new_data;
        return vec;
    }
}

void*
vector_get(const Vector* vec, size_t idx)
{
    return vec->data + (vec->element_size * idx);
}

/**
 * @brief vector_push an argument onto a vector
 * @param vec to vector_push arg to
 * @param arg to vector_push onto vector
 * @return new pointer to the pushed argument
 */
Vector*
vector_push(Vector* vec, void* arg)
{
    vec = vector_grow(vec);
    memcpy(vector_get(vec, vec->size), arg, vec->element_size);
    vec->size += 1;

    return vec;
}

/**
 * @brief vector_pop an element off a vector
 * @param vec to vector_pop off an argument from
 * @return pointer to argument that was popped off
 */
void*
vector_pop(Vector* vec)
{
    void* element = vector_get(vec, vec->size - 1);
    vec->size -= 1;

    return element;
}
