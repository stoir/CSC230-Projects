/**
 * @file catalog.h
 * @author Mike Babb
 * @author Sanjana Cheerla
 * @brief catalog to maintain books and perform all operations
 * related to catalog involving creating and listing books
 */

#ifndef catalog_h
#define catalog_h

#include "input.h"
#include "vector/vector.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Title length of books
 */
#define TITLE_LENGTH 38

/**
 * length of author name
 */
#define AUTHOR_LENGTH 20

/**
 * dots for books and authors that are too long
 */
#define DOTS ".."

/**
 * length of dots
 */
#define DOTS_LEN strlen(DOTS)

/**
 * Book struct consisting of an ID, titile, author, level and word count
 */
typedef struct {
    int id;
    char title[TITLE_LENGTH + 1]; // stores 38 + 1 for null terminator
    char author[AUTHOR_LENGTH + 1];
    double level;
    int words;
    char* subject;
} Book;

/**
 * Catalog of books
 */
typedef struct {
    Vector books;
} Catalog;

/**
 * @brief create a catalog
 * @return created catalog
 */
Catalog*
makeCatalog(void);

/**
 * @brief free catalog memory
 * @param cat catalog to free
 */
void
freeCatalog(Catalog* cat);

/**
 * @brief read books into catalog
 * @param cat to read books into
 * @param filename of book information
 */
void
readCatalog(Catalog* cat, char const* filename);

/**
 * @brief list all books in the catalog
 * @param cat catalog of books to list
 */
void
listAll(Catalog* cat);

void
listReadingList(Catalog* cat);

/**
 * @brief list all books ranging from min level to max level
 * @param cat of books
 * @param min level
 * @param max level
 */
void
listLevel(Catalog* cat, double min, double max);

/**
 * @brief list catalog static, print any selected books
 * @param cat catalog
 * @param test function
 * @param data of books to print
 */
void
listCatalog(Catalog* cat,
            bool (*test)(Book const* book, void const* data),
            void const* data);

/**
 * @brief add a book in the catalog to the reading list
 * @param reading_list to add a book to
 * @param id_num of book to add
 */
void
addBook(Catalog* cat, Catalog* reading_list, int id_num);

/**
 * @brief delete a book from a reading list catalog
 * @param reading_list to delete a book from
 * @param id_num of book to delete
 * @return deleted book
 */
Book*
deleteBook(Catalog* reading_list, int id_num);

char*
strdup2(const char* src);

Vector
strsplit2(char* line, char* delimiter);

void
listSubject(Catalog* cat, char const* subject);

#endif /* catalog_h */
