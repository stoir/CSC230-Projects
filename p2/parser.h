/**
 * @file parser.h
 * @author Mike Babb (mbabb),  Sanjana Cheerla (scheerl)
 * Parses the ppm file and turns it into a PPM image described in image.h
 */
#ifndef parser_h
#define parser_h

#include "image.h"
#include "lexer.h"

#include <stdio.h>

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
 * parses the ppm image
 * @return a parsed ppm image
 */
PPMImage
parse_ppm(FILE* fp);

#endif
