/**
 * @file catalog.c
 * @author Mike Babb
 * @author Sanjana Cheerla
 * @brief catalog to maintain books and perform all operations
 * related to catalog involving creating and listing books
 */

#include "catalog.h"

#include "vector/vector.h"

#define BOOK_VAL 5

/**
 * 2nd field of book
 */
#define AUTHOR 2

/**
 * 3rd field of book
 */
#define LEVEL 3

/**
 * 4th field of book
 */
#define WORDS 4

/**
 * @brief create a catalog
 * @return created catalog
 */
Catalog*
makeCatalog()
{
    Catalog* catalog = (Catalog*) malloc(sizeof(Catalog));
    catalog->books = vector_create(sizeof(Book*));
    return catalog;
}

/**
 * @brief free catalog memory
 * @param cat catalog to free
 */
void
freeCatalog(Catalog* cat)
{
    for (int i = 0; i < cat->books.size; i++) {
        Book* book = *(Book**) vector_get(&cat->books, i);
        free(book->subject);
        free(book);
    }
    vector_destroy(&cat->books);
    free(cat);
}

/**
 * compare val 1 and 2 as ints
 * @param val1 to compare
 * @param val2 to compare
 * @return -1 if val1 is < val 2, 1 if otherwise, and 2 if equal
 */
int
int_compare(int val1, int val2)
{
    if (val1 < val2) {
        return -1;
    } else if (val1 > val2) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * compare val 1 and 2 as doubles
 * @param val1 to compare
 * @param val2 to compare
 * @return -1 if val1 is < val 2, 1 if otherwise, and 2 if equal
 */
int
double_compare(double val1, double val2)
{
    if (val1 < val2) {
        return -1;
    } else if (val1 > val2) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * compare val 1 and 2 by ids
 * @param p1 to compare
 * @param p2 to compare
 * @return -1 if p1 is < p2, 1 if otherwise, and 2 if equal
 */
int
compareByID(const void* p1, const void* p2)
{
    Book* b1 = *(Book**) p1;
    Book* b2 = *(Book**) p2;

    return int_compare(b1->id, b2->id);
}

/**
 * compare val 1 and 2 by level thhen by id
 * @param p1 to compare
 * @param p2 to compare
 * @return -1 if p1 is < p2, 1 if otherwise, and 2 if equal
 */
static int
compareByLevel(const void* p1, const void* p2)
{

    const Book* b1 = *(Book**) p1;
    const Book* b2 = *(Book**) p2;

    const int level_comp = double_compare(b1->level, b2->level);
    const int id_comp = int_compare(b1->id, b2->id);

    if (level_comp != 0) {
        return level_comp;
    } else {
        return id_comp;
    }
}

/**
 * get a book by id
 * @param cat to get book from
 * @param id of book
 * @return book
 */
Book*
get_book_by_id(Catalog* cat, int id)
{
    for (int i = 0; i < cat->books.size; i++) {
        Book* t_book = *(Book**) vector_get(&cat->books, i);

        if (t_book->id == id) {
            return t_book;
        }
    }
    return NULL;
}

char*
strdup2(const char* src)
{
    if (src != NULL) {
        char* dest = malloc(strlen(src) * sizeof(char) + 1);
        if (dest != NULL) {
            strcpy(dest, src);
        }
        return dest;
    } else {
        return NULL;
    }
}

Vector
strsplit2(char* line, char* delimiter)
{
    char* t_line = (char*) malloc(strlen(line) * sizeof(char) + 1);
    strcpy(t_line, line);
    char* t_line_start = t_line;

    Vector comps = vector_create(sizeof(char*));

    while (true) {
        char* token = strtok(t_line, delimiter);

        if (token == NULL) {
            break;
        } else {
            char* tmp = strdup2(token);
            vector_push(&comps, &tmp);
            t_line = NULL;
        }
    }

    free(t_line_start);
    return comps;
}

char*
clamp_str(char* src, int max_len, char* clamp_value)
{
    int len = strlen(src);
    int ix = max_len - strlen(clamp_value);

    if (len > max_len && ix > 0 && ix < len) {
        strcpy(&src[ix], clamp_value);
    }

    return src;
}

Book*
book_create(char* line)
{
    Book* book = (Book*) malloc(sizeof(Book));
    Vector line_comps = strsplit2(line, "\t");

    if (line_comps.size < BOOK_VAL) {
        goto fail;
    } else {
        char* end;

        char* id_str = *(char**) vector_get(&line_comps, 0);

        char* title = *(char**) vector_get(&line_comps, 1);
        char* author = *(char**) vector_get(&line_comps, AUTHOR);

        char* level_str = *(char**) vector_get(&line_comps, LEVEL);
        char* words_str = *(char**) vector_get(&line_comps, WORDS);

        char* subject = line_comps.size == BOOK_VAL + 1
                          ? *(char**) vector_get(&line_comps, BOOK_VAL)
                          : NULL;

        int id = (int) strtod(id_str, &end);
        if (end == id_str) {
            goto fail;
        }

        double level = strtod(level_str, &end);
        if (end == level_str) {
            goto fail;
        }

        int words = (int) strtod(words_str, &end);
        if (end == words_str) {
            goto fail;
        }

        clamp_str(title, TITLE_LENGTH, DOTS);
        clamp_str(author, AUTHOR_LENGTH, DOTS);

        book->id = id;

        strcpy(book->title, title);
        strcpy(book->author, author);

        book->level = level;
        book->words = words;

        book->subject = strdup2(subject);

        goto success;
    }
fail:
    free(book);
    for (int i = 0; i < line_comps.size; i++) {
        free(*(char**) vector_get(&line_comps, i));
    }
    vector_destroy(&line_comps);
    return NULL;
success:
    for (int i = 0; i < line_comps.size; i++) {
        free(*(char**) vector_get(&line_comps, i));
    }
    vector_destroy(&line_comps);
    return book;
}

/**
 * @brief read books into catalog
 * @param cat to read books into
 * @param filename of book information
 */
void
readCatalog(Catalog* cat, char const* filename)
{
    FILE* fp = fopen(filename, "r");

    if (fp == NULL) {
        fprintf(stderr, "Can't open file: %s\n", filename);
        freeCatalog(cat);
        exit(1);
    }

    while (true) {
        char* line = readLine(fp);

        if (line == NULL) {
            break;
        }

        Book* book = book_create(line);
        free(line);

        if (book == NULL) {
            fprintf(stderr, "Invalid book list: %s\n", filename);
            goto fail;
        }

        if (get_book_by_id(cat, book->id) != NULL) {
            fprintf(stderr, "Duplicate book id: %d\n", book->id);
            goto fail;
        } else {
            vector_push(&cat->books, &book);
        }
    }
    goto success;

fail:
    fclose(fp);
    freeCatalog(cat);
    exit(1);
success:
    fclose(fp);
}

/**
 * comparator
 */
typedef int (*Comparator)(const void*, const void*);

/**
 * @brief sort the vatalog by comparator
 * @param cat to sort
 * @param comp comparator
 */
void
sort_catalog(Catalog* cat, Comparator comp)
{
    qsort(cat->books.data, cat->books.size, cat->books.element_size, comp);
}

/**
 * @brief list all predicate to list all values
 * @param book to list all
 * @param data of book
 * @return true
 */
bool
list_all_predicate(Book const* book, void const* data)
{
    return true;
}

/**
 * @brief list all books in the catalog
 * @param cat catalog of books to list
 */
void
listAll(Catalog* cat)
{
    sort_catalog(cat, compareByID);
    listCatalog(cat, list_all_predicate, NULL);
}

/**
 * @brief list level predicate to list all values
 * @param book to list all
 * @param data of book
 * @return true if book level is between min and max, false otherwise
 */
bool
list_subject_predicate(Book const* book, void const* data)
{
    char* subject = *(char**) data;

    char* book_subject = strstr(book->subject, subject);

    return book_subject != NULL;
}

void
listSubject(Catalog* cat, char const* subject)
{
    Vector vec = vector_create(sizeof(char*));
    vector_push(&vec, &subject);

    sort_catalog(cat, compareByID);
    listCatalog(cat, list_subject_predicate, vec.data);

    vector_destroy(&vec);
}

/**
 * @brief list level predicate to list all values
 * @param book to list all
 * @param data of book
 * @return true if book level is between min and max, false otherwise
 */
bool
list_level_predicate(Book const* book, void const* data)
{
    double min = *(double*) (data + 0);
    double max = *(double*) (data + 1 * sizeof(double*));

    double level = book->level;

    if (level < min || level > max) {
        return false;
    } else {
        return true;
    }
}

/**
 * @brief list levels of books
 * @param cat catalog
 * @param min of levels
 * @param max of levels
 */
void
listLevel(Catalog* cat, double min, double max)
{
    Vector vec = vector_create(sizeof(double));
    vector_push(&vec, &min);
    vector_push(&vec, &max);

    sort_catalog(cat, compareByLevel);
    listCatalog(cat, list_level_predicate, vec.data);

    vector_destroy(&vec);
}

/**
 * @brief list catalog static, print any selected books
 * @param cat catalog
 * @param test function
 * @param data of books to print
 */
void
listReadingList(Catalog* cat)
{
    double level_sum = 0;
    int words_sum = 0;

    for (int i = 0; i < cat->books.size; i++) {
        Book* book = *(Book**) vector_get(&cat->books, i);
        level_sum += book->level;
        words_sum += book->words;
    }

    listCatalog(cat, list_all_predicate, NULL);

    const char* footer = "                                                     "
                         "             %5.1f %7d\n";

    int size = cat->books.size;
    printf(footer, level_sum / size, words_sum);
}

void
listCatalog(Catalog* cat,
            bool (*test)(Book const* book, void const* data),
            void const* data)
{
    const char* header = "   ID                                  Title         "
                         "      Author Level   Words\n";

    const char* fmt_string = "%5d %38s %20s %5.1lf %7d\n";

    int print_count = 0;

    for (int i = 0; i < cat->books.size; i++) {
        Book* book = *(Book**) vector_get(&cat->books, i);
        bool to_print = test(book, data);

        print_count = to_print ? print_count + 1 : print_count;
    }

    if (print_count == 0) {
        printf("No matching books\n");
    } else {
        printf("%s", header);

        for (int i = 0; i < cat->books.size; i++) {
            Book* book = *(Book**) vector_get(&cat->books, i);

            bool to_print = test(book, data);

            if (to_print) {
                printf(fmt_string,
                       book->id,
                       book->title,
                       book->author,
                       book->level,
                       book->words);
            }
        }
    }
}

/**
 * @brief delete a book from a reading list catalog
 * @param reading_list to delete a book from
 * @param id_num of book to delete
 * @return deleted book
 */
Book*
deleteBook(Catalog* reading_list, int id_num)
{
    int remove_ix = -1;

    for (int i = 0; i < reading_list->books.size; i++) {
        Book* book = *(Book**) vector_get(&reading_list->books, i);

        if (book->id == id_num) {
            remove_ix = i;
            break;
        }
    }

    if (remove_ix > -1) {
        return vector_remove(&reading_list->books, remove_ix);
    } else {
        printf("Book %d is not on the reading list\n", id_num);
        return NULL;
    }
}

/**
 * @brief add a book in the catalog to the reading list
 * @param reading_list to add a book to
 * @param id_num of book to add
 */
void
addBook(Catalog* cat, Catalog* reading_list, int id_num)
{
    Book* book = get_book_by_id(cat, id_num);

    if (book == NULL) {
        printf("Book %d is not in the catalog\n", id_num);
    } else {
        if (get_book_by_id(reading_list, id_num) != NULL) {
            printf("Book %d is already on the reading list\n", id_num);
        } else {
            vector_push(&reading_list->books, &book);
        }
    }
}
