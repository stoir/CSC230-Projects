/**
 * @file lexer.h
 * @author Mike Babb (mbabb),  Sanjana Cheerla (scheerl)
 * handles all tokens for the lexer used in parser
 */
#ifndef lexer_h
#define lexer_h

#include <stdio.h>

/**
 * buffer size
 */
#define BUFF_SIZE 256

/**
 * enum for the type of token
 */
typedef enum
{
    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_EOF,
    TOKEN_ERROR,

    TOKEN_DOT,
    TOKEN_MINUS,
} TokenType;

/**
 * tokwn type struct
 */
typedef struct
{
    TokenType token_type;
    int start;
    int length;
    int line;
    char* value;
} Token;

/**
 * lexer struct
 */
typedef struct
{
    char current_char;
    char buffer[BUFF_SIZE];

    int start;
    int current;

    int column;
    int line;

    FILE* source_file;
} Lexer;

/**
 * @brief create a lexer
 * @param fp to start the lexer from
 * @return created lexer
 */
Lexer
lexer_create(FILE* fp);

/**
 * @brief iterates through the token string
 * @param lexer to iterate
 * @return the created token
 */
Token
lex(Lexer* lexer);

// char*
// token_get_value(Lexer* lexer, Token* token);

/**
 * @brief destroy the token
 * @param token being destroyed
 */
void
token_destroy(Token* token);

#endif
