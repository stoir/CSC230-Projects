/**
 * @file lexer.c
 * @author Mike Babb (mbabb),  Sanjana Cheerla (scheerl)
 * handles all tokens for the lexer used in parser
 */
#include "lexer.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief checks to see if  char is an alpha
 * @param c being checked
 * @return true if c is alpha, false otherwise
 */
bool
is_alpha(const char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

/**
 * @brief checks to see if  char is a digit
 * @param c being checked
 * @return true if c is digit, false otherwise
 */
bool
is_digit(const char c)
{
    return c >= '0' && c <= '9';
}

/**
 * @brief makes a token using lexer and token types
 * @param lexer being used to make token
 * @param token_type for the token
 * @return the created token
 */
Token
make_token(Lexer* lexer, TokenType token_type)
{

    const int length = lexer->current - lexer->start;
    char* buff = malloc(length + 1);

    strncpy(buff, lexer->buffer, length);
    buff[length] = '\0';

    Token token = {.token_type = token_type,
                   .start = lexer->start,
                   .length = length,
                   .line = lexer->line,
                   .value = buff};

    return token;
}

/**
 * @brief destroy the token
 * @param token being destroyed
 */
void
token_destroy(Token* token)
{
    free(token->value);
    token = NULL;
}

/**
 * @brief check to see if lexer is at end
 * @param lexer being chcked
 * @return true if lexer is at end, false otherwise
 */
bool
lexer_is_at_end(Lexer* lexer)
{
    return lexer->current_char == '\0' || lexer->current_char == EOF;
}

/**
 * @brief advance lexer
 * @param lexer being advanced
 * @return next character
 */
char
lexer_advance(Lexer* lexer)
{
    char previous = lexer->current_char;
    lexer->buffer[lexer->current] = previous;

    lexer->current_char = getc(lexer->source_file);
    lexer->current += 1;
    return previous;
}

/**
 * @brief peek forward in lexer
 * @param lexer being peeked
 * @param count to peek at
 * @return char at peek
 */
char
lexer_peek(Lexer* lexer, int count)
{
    if (lexer_is_at_end(lexer)) {
        return '\0';
    } else {
        char buff[count + 1];
        buff[0] = lexer->current_char;

        for (int i = 1; i < count; i++) {
            buff[i] = getc(lexer->source_file);
        }
        for (int i = count - 1; i > 0; i--) {
            ungetc(buff[i], lexer->source_file);
        }

        return buff[count];
    }
}

/**
 * @brief get the lexer number and make a token from it
 * @param lexer to make token from
 * @return numeric token
 */
Token
lexer_number(Lexer* lexer)
{
    while (is_digit(lexer_peek(lexer, 0))) {
        lexer_advance(lexer);
    };
    return make_token(lexer, TOKEN_NUMBER);
}

/**
 * @brief create a string token
 * @param lexer being created
 * @return create a string tttoken
 */
Token
lexer_string(Lexer* lexer)
{
    while (1) {
        const char c = lexer_peek(lexer, 0);
        if (c == ' ' || c == '\t' || c == '\n' || c == '\0') {
            break;
        } else {
            lexer_advance(lexer);
        }
    }
    return make_token(lexer, TOKEN_STRING);
}

/**
 * @brief check to see if lexer is at whitespace
 * @param lexer being checked
 */
void
lexer_whitespace(Lexer* lexer)
{
    while (1) {
        const char c = lexer_peek(lexer, 0);

        switch (c) {
            case ' ':
            case '\t':
            case '\r':
                lexer_advance(lexer);
                break;
            case '\n':
                lexer->line += 1;
                lexer_advance(lexer);
                break;
            case '#':
                while (!lexer_is_at_end(lexer) &&
                       lexer_peek(lexer, 0) != '\n') {
                    lexer_advance(lexer);
                };
                break;
            default:
                return;
        }
    }
}

/**
 * @brief create a lexer
 * @param fp to start the lexer from
 * @return created lexer
 */
Lexer
lexer_create(FILE* fp)
{
    Lexer lexer = {.current_char = ' ',
                   .start = 0,
                   .current = 0,
                   .column = 0,
                   .line = 0,

                   .source_file = fp};
    return lexer;
}

/**
 * @brief iterates through the token string
 * @param lexer to iterate
 * @return the created token
 */
Token
lex(Lexer* lexer)
{
    if (lexer->source_file == NULL) {
        return make_token(lexer, TOKEN_ERROR);
    } else if (feof(lexer->source_file)) {
        return make_token(lexer, TOKEN_EOF);
    }

    lexer_whitespace(lexer);

    lexer->start = 0;
    lexer->current = 0;

    if (lexer_is_at_end(lexer)) {
        return make_token(lexer, TOKEN_EOF);
    } else {
        const char c = lexer_peek(lexer, 0);

        if (is_alpha(c)) {
            return lexer_string(lexer);
        } else if (is_digit(c)) {
            return lexer_number(lexer);
        } else {
            // Super simple grammar: everything is a string...
            return lexer_string(lexer);
            // lexer->current += 1;
            // return make_token(lexer, TOKEN_ERROR);
        }
    }
}
