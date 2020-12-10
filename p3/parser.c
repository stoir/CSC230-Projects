/**
 * @file parser.c
 * @author Mike Babb (mbabb),  Sanjana Cheerla (scheerl)
 * Parses the ppm file and turns it into a PPM image described in image.h
 */
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief checks to see if the parser is at the end of the file
 * @param parser the parser being checked
 * @return true if the parser is at the end
 * false otherwise
 */
bool
parser_is_at_end(Parser* parser)
{
    return parser->current.token_type == TOKEN_EOF;
}

/**
 * @brief advance the parser
 * @param parser the parser being advanced
 * @return the advanced parser
 */
Token
parser_advance(Parser* parser)
{
    Token current = parser->current;

    if (current.token_type == TOKEN_ERROR) {
        printf("Parser error at token %s, column %d, line %d",
               current.value,
               current.start,
               current.line);
        exit(EXIT_FAILURE);
    } else {
        if (!parser_is_at_end(parser)) {
            parser->previous = current;
            parser->current = lex(parser->lexer);

            return parser->previous;
        } else {
            return current;
        }
    }
}

/**
 * @brief create a parser using lexer
 * @param lexer being used to create a parser image
 * @return the parser
 */
Parser
parser_create(Lexer* lexer)
{
    Parser parser;
    parser.lexer = lexer;
    return parser;
}
