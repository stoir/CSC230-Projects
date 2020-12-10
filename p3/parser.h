/**
 * @file parser.h
 * @author Mike Babb (mbabb),  Sanjana Cheerla (scheerl)
 * Parses the ppm file and turns it into a PPM image described in image.h
 */
#ifndef parser_h
#define parser_h

#include "lexer.h"

#include <stdio.h>
#include <stdbool.h>

/**
 * parser struct which has current token, previous token, and lexer
 */
typedef struct
{
    Lexer* lexer;
    Token current;
    Token previous;
} Parser;

/**
 * @brief checks to see if the parser is at the end of the file
 * @param parser the parser being checked
 * @return true if the parser is at the end
 * false otherwise
 */
bool
parser_is_at_end(Parser* parser);

/**
 * @brief advance the parser
 * @param parser the parser being advanced
 * @return the advanced parser
 */
Token
parser_advance(Parser* parser);

/**
 * @brief check to see if the token is a valid number
 * @param parser the parser to check the token from
 * @return the token value
 */
int
parser_number(Parser* parser);

/**
 * @brief create a parser using lexer
 * @param lexer being used to create a parser image
 * @return the parser
 */
Parser
parser_create(Lexer* lexer);

#endif
