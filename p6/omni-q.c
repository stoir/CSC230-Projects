/**
 * @file omni-q.c
 * @author Mike Babb
 * @author Sanjana Cheerla
 * @brief omni queue main class
 */
#include "data.h"
#include "vector/vector.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VECTOR_INIT_SZ 16
#define COMMAND_SZ 20

/**
 * @brief joins a delimiter to the vector
 * @param vec  to join to
 * @param delim to join
 * @return the joined string
 */
char*
vector_join(Vector* vec, char* delim)
{
    size_t size = vec->size;
    size_t total_size = strlen(delim) * (size - 1) + size;

    char* str = (char*) calloc(total_size + 1, sizeof(char));
    int ix = 0;

    for (int i = 0; i < size; i++) {
        char ch = *(char*) vector_get(vec, i);
        str[ix] = ch;
        ix += 1;

        for (int j = 0; j < strlen(delim); j++) {
            str[ix] = delim[j];
            ix += 1;
        }
    }

    ix += 1;
    str[ix] = '\0';
    return str;
}

/**
 * @brief consumes characters until delim
 * @param fp to consume from
 * @param delim to read upto
 * @return string read in
 */
char*
consume_until(FILE* fp, char delim)
{

    Vector vec = vector_create(sizeof(char), VECTOR_INIT_SZ);

    char ch;
    while (EOF != (ch = fgetc(fp)) && ch != delim) {
        vector_push(&vec, &ch);
    }

    char* out = vector_join(&vec, "");
    vector_destroy(&vec);

    return out;
}

/**
 * @brief main program to parse and perform commands
 * @param argc not used
 * @param argv not used
 * @return exit_success or exit_fail
 */
int
main(int argc, char* argv[])
{
    char command[COMMAND_SZ];
    char type[COMMAND_SZ];

    GenericList* list = makeList();

    while (true) {
        int scanfVal = scanf(" %s", command);

        if (scanfVal == EOF) {
            goto success;
        } else if (scanfVal != 1) {
            goto fail;
        }

        bool enqueueMatch = strcmp(command, "enqueue") == 0;
        bool pushMatch = strcmp(command, "push") == 0;
        bool popMatch = strcmp(command, "pop") == 0;

        if (!(enqueueMatch || pushMatch || popMatch)) {
            goto fail;
        }

        if (popMatch) {
            Node* node = pop(list);

            if (node != NULL) {
                node->print(node);
                node->destroy(node);
            }
            continue;
        }

        if (enqueueMatch || pushMatch) {
            scanfVal = scanf(" %s ", type);

            if (scanfVal == EOF || scanfVal != 1) {
                goto fail;
            }

            bool intMatch = strcmp(type, "int") == 0;
            bool realMatch = strcmp(type, "real") == 0;
            bool strMatch = strcmp(type, "string") == 0;

            if (!(intMatch || realMatch || strMatch)) {
                goto fail;
            }

            if (intMatch) {
                int intElement;
                scanfVal = scanf(" %d", &intElement);

                if (scanfVal == EOF || scanfVal != 1) {
                    goto fail;
                }

                Node* intNode = makeIntNode(intElement);

                if (enqueueMatch) {
                    enqueue(list, intNode);
                } else {
                    push(list, intNode);
                }
                continue;
            }

            if (realMatch) {
                double realElement;
                scanfVal = scanf(" %lf", &realElement);

                if (scanfVal == EOF || scanfVal != 1) {
                    goto fail;
                }

                Node* realNode = makeRealNode(realElement);

                if (enqueueMatch) {
                    enqueue(list, realNode);
                } else {
                    push(list, realNode);
                }

                continue;
            }

            if (strMatch) {
                char* strElement = consume_until(stdin, '\n');

                Node* strNode = makeStringNode(strElement);

                if (enqueueMatch) {
                    enqueue(list, strNode);
                } else {
                    push(list, strNode);
                }

                free(strElement);

                continue;
            }
        }
    }

fail:
    freeList(list);
    return EXIT_FAILURE;
success:
    freeList(list);
    return EXIT_SUCCESS;
}
