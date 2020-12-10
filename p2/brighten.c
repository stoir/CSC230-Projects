/**
 * @file brighten.c
 * @author Mike Babb (mbabb),  Sanjana Cheerla (scheerl)
 * Brightens the image by a value of 32 on all values in the ppm image
 */
#include "image.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * minimum rgb pixel value
 */
#define MIN 0
/**
 * maximum rgb pixel value
 */
#define MAX 255
/**
 * brighten rgb pixel value
 */
#define BRIGHTEN 32

/**
 * @brief brightens the image by a value of 32 on all values in the ppm image
 * @return 100 if the ppm header is not p3 or if the ppm max intensity is not
 * 255 or if width or height in the ppm is less than or equal to 0 101 if there
 * is any invalid values in the pixel values for the image EXIT_SUCCESS if the
 * image is brightened successfully
 */
int
main()
{
    FILE* in_file = stdin;
    FILE* out_file = stdout;
    PPMImage img = parse_ppm(in_file);

    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            RGB rgb = img.data[i][j];
            rgb_add_scalar(BRIGHTEN, &rgb);
            rgb_clamp(&rgb, MIN, MAX);
            
            img.data[i][j] = rgb;
        }
    }

    image_print(&img, out_file);

    image_destroy(&img);
    return EXIT_SUCCESS;
}
