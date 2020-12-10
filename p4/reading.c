/**
 * @file reading.c
 * @author Mike Babb
 * @author Sanjana Cheerla
 * @brief main program for proj4
 */
#include "catalog.h"
#include "input.h"
#include "vector/vector.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**
 * valid command count for level command
 */
#define VALID_COMMAND_COUNT 2

/**
 * @brief prints invalid command
 */
static void
printInvalidCommand()
{
    printf("Invalid command\n");
}

/**
 * @brief main program
 * @param argc for number of arguments
 * @param argv of arguments used to read in catalogs
 * @return 0 if successful
 */
int
main(int argc, const char* argv[])
{
    if (argc <= 1) {
        fprintf(stderr, "usage: reading <book-list>*\n");
        exit(1);
    }

    Catalog* cat = makeCatalog();
    Catalog* reading_list = makeCatalog();

    for (int i = 1; i < argc; i++) {
        readCatalog(cat, argv[i]);
    }

    int i = 0;

    while (true) {
        printf("cmd> ");

        char* line = readLine(stdin);

        if (line == NULL || line[0] == '\0') {
            if (i > 0) {
                break;
            } else {
                printf("\n");
                printInvalidCommand();
                printf("\n");
                continue;
            }
        }

        printf("%s\n", line);

        Vector line_comps = strsplit2(line, " ");
        char* cmd = *(char**) vector_get(&line_comps, 0);

        if (strcmp("catalog", line) == 0) {
            if (cat->books.size == 0) {
                printf("No matching books\n");
            } else {
                listAll(cat);
            }
        } else if (strcmp("level", cmd) == 0 && line_comps.size == VALID_COMMAND_COUNT + 1) {
            if (cat->books.size == 0) {
                printf("No matching books\n");
            } else {
                char* end;

                char* min_str = *(char**) vector_get(&line_comps, 1);
                char* max_str = *(char**) vector_get(&line_comps, VALID_COMMAND_COUNT);

                double min = strtod(min_str, &end);
                if (end == min_str) {
                    goto invalid_command;
                }

                double max = strtod(max_str, &end);
                if (end == max_str) {
                    goto invalid_command;
                }

                listLevel(cat, min, max);
            }
        } else if (strcmp("add", cmd) == 0 && line_comps.size == VALID_COMMAND_COUNT) {
            char* end;
            char* id_str = *(char**) vector_get(&line_comps, 1);

            int id = (int) strtod(id_str, &end);
            if (end == id_str) {
                goto invalid_command;
            }

            addBook(cat, reading_list, id);
        } else if (strcmp("remove", cmd) == 0 && line_comps.size == VALID_COMMAND_COUNT) {
            char* end;
            char* id_str = *(char**) vector_get(&line_comps, 1);

            int id = (int) strtod(id_str, &end);
            if (end == id_str) {
                goto invalid_command;
            }

            deleteBook(reading_list, id);
        } else if (strcmp("list", line) == 0) {
            if (reading_list->books.size == 0) {
                printf("List is empty\n");
            } else {
                listReadingList(reading_list);
            }
        } else if (strcmp("quit", line) == 0) {
            break;
        } else if (strcmp("subject", cmd) == 0 && line_comps.size == VALID_COMMAND_COUNT) {
            char* subject = *(char**) vector_get(&line_comps, 1);

            listSubject(cat, subject);
        } else {
        invalid_command:
            printInvalidCommand();
        }

        printf("\n");
        i += 1;

        free(line);
        for (int j = 0; j < line_comps.size; j++) {
            free(*(char**) vector_get(&line_comps, j));
        }
        vector_destroy(&line_comps);
    }

    freeCatalog(cat);
    vector_destroy(&reading_list->books);
    free(reading_list);

    exit(0);
}
