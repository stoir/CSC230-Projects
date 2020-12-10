/**
 * @file border.c
 * @author Mike Babb (mbabb),  Sanjana Cheerla (scheerl)
 * Adds a 4 pixel width border around a valid ppm image
 */
#include "image.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * minimum rgb pixel value
 */
#define RGB_MIN 0
/**
 * maximum rgb pixel value
 */
#define RGB_MAX 255
/**
 * border value for border
 */
#define BORDER 4

/**
 * @brief adds a 4 pixel black border around the ppm image
 * @return 100 if the ppm header is not p3 or if the ppm max intensity is not
 * 255 or if width or height in the ppm is less than or equal to 0 101 if there
 * is any invalid values in the pixel values for the image EXIT_SUCCESS if the
 * image has a border added successfully
 */
int
main()
{
    FILE* in_file = stdin;
    FILE* out_file = stdout;

    PPMImage img = parse_ppm(in_file);

    RGB black = {RGB_MIN, RGB_MIN, RGB_MIN};

    PPMImage new_image = image_square_pad(BORDER, BORDER, &img, &black);
    image_print(&new_image, out_file);

    image_destroy(&new_image);

    return EXIT_SUCCESS;
}
