/**
 * @file parser.c
 * @author Mike Babb (mbabb),  Sanjana Cheerla (scheerl)
 * Parses the ppm file and turns it into a PPM image described in image.h
 */
#include "parser.h"

#include "image.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * header error exit statue
 */
#define ERROR_HEADER 100

/**
 * rgb error exit status
 */
#define ERROR_RGB 101

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
    if (!parser_is_at_end(parser)) {
        parser->previous = parser->current;
        parser->current = lex(parser->lexer);

        return parser->previous;
    } else {
        return parser->current;
    }
}

/**
 * @brief checks to see if the read in value by parser is a number
 * @param parser being checked
 * @return 0 with the exit status 101 if the parser is not a number
 * or return the integer value of parser
 */
int
unary(Parser* parser)
{
    Token left = parser_advance(parser);

    if (left.token_type != TOKEN_NUMBER) {
        exit(ERROR_RGB);
        return 0;
    } else {
        int out = atoi(left.value);

        token_destroy(&left);
        return out;
    }
}

/**
 * @brief parses the header for the ppm image
 * @param parser for creating the header for the ppm image
 * @return 100 if there is an error creating the parser, data is not valid
 * the created ppm image with the header if the parser is valid
 */
PPMImage
parse_header(Parser* parser)
{
    Token magic_number_token = parser_advance(parser);

    Token width_token = parser_advance(parser);
    Token height_token = parser_advance(parser);
    Token max_intensity_token = parser_advance(parser);

    char* magic_number = magic_number_token.value;
    int height = atoi(height_token.value);
    int width = atoi(width_token.value);
    int max_intensity = atoi(max_intensity_token.value);

    if (strcmp(magic_number, "P3") != 0 || height < 1 || width < 1 ||
        max_intensity != 255) {
        exit(ERROR_HEADER);
    } else {
        PPMImage image =
          image_create(magic_number, height, width, max_intensity);

        token_destroy(&height_token);
        token_destroy(&width_token);
        token_destroy(&max_intensity_token);

        return image;
    }
}

/**
 * @brief parses the rgb for the PPM image
 * @param parser to check for the values
 * @return 0 the exit status 101 if the parser is not a number
 * or return the RGB struct
 */
RGB
parse_rgb(Parser* parser)
{
    Token previous = parser->current;

    int red = unary(parser);
    int green = unary(parser);
    int blue = unary(parser);

    if (previous.line != parser->previous.line || !in_range(red, 0, 256) ||
        !in_range(green, 0, 256) || !in_range(blue, 0, 256)) {
        printf("Error at column %d, line %d\n", previous.start, previous.line);

        exit(ERROR_RGB);
    } else {
        RGB rgb = {.red = red, .green = green, .blue = blue};
        return rgb;
    }
}

/**
 * @brief parse the data with the ppm image
 * @param parser to parse the data
 * @param image tthe created image
 * @return the created ppm valid image
 * or 101 if there are any less or more rgb values
 */
PPMImage*
parse_body(Parser* parser, PPMImage* image)
{

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            RGB rgb = parse_rgb(parser);
            image->data[i][j] = rgb;
        }
    }

    if (parser->current.token_type != TOKEN_EOF) {
        printf("Parsed image height does not match actual height!\n");
        exit(ERROR_HEADER);
    }
    return image;
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

/**
 * parses the ppm image
 * @return a parsed ppm image
 */
PPMImage
parse_ppm(FILE* fp)
{
    Lexer lexer = lexer_create(fp);
    Parser parser = parser_create(&lexer);

    parser_advance(&parser);

    PPMImage image = parse_header(&parser);
    parse_body(&parser, &image);

    return image;
}
