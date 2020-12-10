/**
 * @file main implementation to edit the files
 * @author Mike Babb (mbabb),  Sanjana Cheerla (scheerl)
 * edit the data to remove or edit specific lines and colums
 */
#include "edit.h"
#include "parser.h"
#include "text.h"
#include "vector.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define THREE 3
#define FOUR 4

/**
 * tmp file name for command line arguments
 */
#define TMP_FILENAME "__tmp.txt"

/**
 * tmp string for invalid arguments
 */
#define INVALID_ARG                                                            \
    "invalid arguments\nusage: chop command* (infile|-) (outfile|-)\n"

/**
 * file string data
 */
extern char FILE_STRING[MAX_ROWS][MAX_COLS];

/**
 * row count
 */
extern int ROW_COUNT;

/**
 * struct for a argument consisting of (line)s/(col)s, and n (row)s and m
 * (col)s.
 */
typedef struct {
    char* name;
    int start;
    int end;
} Argument;

/**
 * @brief parses the number value for sttart and end
 * @param parser to parse the number from
 * @return tthe numerical value, or else fails
 */
int
parser_number(Parser* parser)
{
    Token token = parser_advance(parser);

    if (token.token_type == TOKEN_NUMBER) {
        int value = atoi(token.value);
        if (value < 0) {
            goto fail;
        } else {
            token_destroy(&token);
            return value;
        }
    } else {
        goto fail;
    }

fail:
    token_destroy(&token);
    fprintf(stderr, INVALID_ARG);
    exit(EXIT_FAILURE);
}

/**
 * @brief checks to see if the arg name is valid
 * @param arg_name to check
 * @return true if the arg name is valid, false otherwise
 */
bool
is_list_arg(const char* arg_name)
{
    return (strcmp(arg_name, "line") == 0 || strcmp(arg_name, "lines") == 0) ||
           (strcmp(arg_name, "col") == 0 || strcmp(arg_name, "cols") == 0);
}

/**
 * @brief parse the argument from command line
 * @param parser to read from
 * @return the created argument
 */
Argument
parse_args(Parser* parser)
{
    Token arg_token = parser_advance(parser);
    char* arg_name = arg_token.value;
    Argument arg = {"\0", -1, -1};

    if (arg_token.token_type == TOKEN_STRING) {
        if (is_list_arg(arg_name)) {
            int start = parser_number(parser);
            int end = arg_name[FOUR] == 's' || arg_name[THREE] == 's'
                        ? parser_number(parser)
                        : start;
            arg.start = start;
            arg.end = end;
        }
        arg.name = arg_name;
    } else {
        fprintf(stderr, INVALID_ARG);
        exit(EXIT_FAILURE);
    }
    return arg;
}

/**
 * @brief parse the argument from command line into a vector
 * @param parser to read from
 * @return the created vecttor
 */
Vector
parse_args_and_filename(FILE* fp)
{
    Lexer lexer = lexer_create(fp);
    Parser parser = parser_create(&lexer);

    Vector vec = vector_create(sizeof(Argument));

    parser_advance(&parser);

    while (!parser_is_at_end(&parser)) {
        Argument arg = parse_args(&parser);
        vector_push(&vec, &arg);
    }
    return vec;
}

/**
 * @brief opens the file and returns a file pointer
 * @param filename to open
 * @param mode to open. read or write
 * @return the created file pointer, or exists if the file is not able to open
 */
FILE*
open_file(const char* filename, const char* mode)
{
    FILE* file = fopen(filename, mode);
    if (file == NULL) {
        fprintf(stderr, "Can't open file: %s\n", filename);
        exit(EXIT_FAILURE);
    } else {
        return file;
    }
}

/**
 * @brief based on the vector arguments edit the file
 * @param vec to read arguments from
 */
void
file_string_edit(Vector* vec)
{
    char* output_filename = ((Argument*) vector_pop(vec))->name;
    char* input_filename = ((Argument*) vector_pop(vec))->name;

    FILE* input_file = (strcmp(input_filename, "-") == 0)
                         ? stdin
                         : open_file(input_filename, "r");
    readFile(input_file);

    for (int i = 0; i < vec->size; i++) {
        Argument* arg = (Argument*) vector_get(vec, i);
        char* arg_name = arg->name;

        if (arg->end < arg->start) {
            fprintf(stderr, INVALID_ARG);
            exit(EXIT_FAILURE);
        }

        if (strcmp(arg_name, "line") == 0 || strcmp(arg_name, "lines") == 0) {
            removeLines(arg->start, arg->end);
        } else if (strcmp(arg_name, "col") == 0 ||
                   strcmp(arg_name, "cols") == 0) {
            removeCols(arg->start, arg->end);
        } else {
            fprintf(stderr, INVALID_ARG);
            exit(EXIT_FAILURE);
        }
        free(arg_name);
    }

    FILE* output_file = (strcmp(output_filename, "-") == 0)
                          ? stdout
                          : open_file(output_filename, "w");

    writeFile(output_file);

    free(input_filename);
    free(output_filename);
}

/**
 * @brief main program to edit the files
 * @param argc to get input from
 * @param argv to get command line argumetns from
 * @return 0 indicating exit success
 */
int
main(const int argc, const char* argv[])
{
    // Lexer needs a stream to read from.
    // This is a hack to do just that.
    FILE* arg_fp = open_file(TMP_FILENAME, "w+");
    for (int i = 1; i < argc; i++) {
        fprintf(arg_fp, "%s\n", argv[i]);
    }
    fseek(arg_fp, 0, SEEK_SET);

    Vector vec = parse_args_and_filename(arg_fp);

    file_string_edit(&vec);

    vector_destroy(&vec);

    remove(TMP_FILENAME);

    return EXIT_SUCCESS;
}
