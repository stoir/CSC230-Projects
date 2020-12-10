/**
 * @file border.c
 * @author Mike Babb (mbabb),  Sanjana Cheerla (scheerl)
 * performs a gauussian blur on an image
 */
#include "image.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * The kernel size.
 */
#define SZ 3
/**
 * gaussian max value
 */
#define GAUSS_MAX 4

/**
 * @brief performs a gaussian blue on the ppm image
 * @return 100 if the ppm header is not p3 or if the ppm max intensity is not
 * 255 or if width or height in the ppm is less than or equal to 0 101 if there
 * is any invalid values in the pixel values for the image EXIT_SUCCESS if the
 * image is blurred successfully
 */
int
main()
{
    FILE* in_file =
      fopen("/Users/mkbabb/Programming/csc230-project-2/data/feep.ascii.ppm",
      "r");
    FILE* out_file = fopen("tmp.ppm", "w");

    // FILE* in_file = stdin;
    // FILE* out_file = stdout;

    PPMImage img = parse_ppm(in_file);

    double arr[SZ][SZ] = {{1, 2, 1}, {2, GAUSS_MAX, 2}, {1, 2, 1}};
    PPMImage kernel = image_from_array(SZ, SZ, arr);

    image_convolve(&img, &kernel, true);
    image_print(&img, out_file);

    image_destroy(&img);

    return EXIT_SUCCESS;
}
