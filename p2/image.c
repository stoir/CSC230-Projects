/**
 * @file image.c
 * @author Mike Babb (mbabb),  Sanjana Cheerla (scheerl)
 * handles all functions necessary to crop/border, brighten/darken, and blur on an image
 */
#include "image.h"
#include "parser.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * depth for pix
 */
#define DEPTH 3
/**
 * max rgb value
 */
#define MAX_RGB 255
/**
 * used for dividing by 2
 */
#define HALF 2.0
/**
 * gaussian row and cols size
 */
#define GAUSS_VAL 3
/**
 * gaussian max value
 */
#define GAUSS_MAX 4
/**
 * gaussian matrix for the gaussian blur
 */
double GAUSSIAN[GAUSS_VAL][GAUSS_VAL] = {{1, 2, 1}, {2, GAUSS_MAX, 2}, {1, 2, 1}};
/**
 * pi value
 */
#define M_PI_LOCAL 3.14

/**
 * @brief clamp x between two values min and max
 * @param min clamp value
 * @param max clamp value
 * @return x if x is between min and max, max if x > max, and min if x < min
 */
int
clamp(const int x, const int min, const int max)
{
    if (x < min) {
        return min;
    } else if (x > max) {
        return max;
    } else {
        return x;
    }
}

/**
 * @brief lerps t with min and max
 * @param min for the lerp
 * @param max for the lerp
 */
int
lerp(const int t, const int min, const int max)
{
    return (1 - t) * min + t * max;
}

/**
 * @brief checks to see if x is in between min and max
 * @param x being checked
 * @param min for the range
 * @param max for the range
 * @return true if the value x is >= min or < max
 *
 */
bool
in_range(const int x, const int min, const int max)
{
    return x >= min && x < max;
}

/**
 * @brief Performs a linear mapping for floats of t 'twixt two intervals, [x1, y1] and [x2, y2].
 * Essentially a linear interpolation of the polynomial y = bx + a, satisfying:
 * a + bx1 = x2
 * a + by1 = y2
 * @param t the value being mapped
 * @param x1 in equation above
 * @param y1 in equation above
 * @param x2 in equation above
 * @param y2 in equation above
 * @return a in equation above
 */
float
flinear_map(float t, float x1, float y1, float x2, float y2)
{
    return ((y2 - x2) / (y1 - x1)) * (t - x1) + x2;
}

/**
 * @brief Performs a linear mapping for floats of t 'twixt two intervals, [x1, y1] and [x2, y2].
 * Essentially a linear interpolation of the polynomial y = bx + a, satisfying:
 * a + bx1 = x2
 * a + by1 = y2
 * @param t the value being mapped
 * @param x1 in equation above
 * @param y1 in equation above
 * @param x2 in equation above
 * @param y2 in equation above
 * @return a in equation above
 */
int
ilinear_map(int t, int x1, int y1, int x2, int y2)
{
    return ((y2 - x2) / (y1 - x1)) * (t - x1) + x2;
}

/**
 * @brief return min of a and b
 * @param b being checked
 * @param a being checked
 * @return the min of a and b
 */
int
imin(int b, int a)
{
    return (b < a) ? b : a;
}

/**
 * @brief return max of a and b
 * @param b being checked
 * @param a being checked
 * @return the max of a and b
 */
int
imax(int b, int a)
{
    return (b > a) ? b : a;
}

/**
 * @brief create a copy of the RGB from source to destination
 * @param src of rgb struct
 * @param dest of rgb struct
 * @return the destination rgb with src values in it
 */
RGB*
rgb_copy(const RGB* src, RGB* dest)
{
    dest->red = src->red;
    dest->green = src->green;
    dest->blue = src->blue;
    return dest;
}

/**
 * @brief clamp rgb values using clamp method
 * @param rgb being clamped
 * @param min for clamp
 * @param max for clamp
 * @return rgb that was clamped
 */
RGB*
rgb_clamp(RGB* rgb, int min, int max)
{
    rgb->red = clamp(rgb->red, min, max);
    rgb->green = clamp(rgb->green, min, max);
    rgb->blue = clamp(rgb->blue, min, max);
    return rgb;
}

/**
 * @brief add a scalar value to rgb
 * @param x being added
 * @param rgb that x is being added to
 * @return rgb that was added
 */
RGB*
rgb_add_scalar(int x, RGB* rgb)
{
    rgb->red = rgb->red + x;
    rgb->green = rgb->green + x;
    rgb->blue = rgb->blue + x;
    return rgb;
}

/**
 * @brief multiplyy a scalar value to rgb
 * @param x being multiplied
 * @param rgb that x is being multiplied to
 * @return rgb that was multiplied
 */
RGB*
rgb_mult_scalar(int x, RGB* rgb)
{
    rgb->red = rgb->red * x;
    rgb->green = rgb->green * x;
    rgb->blue = rgb->blue * x;
    return rgb;
}

/**
 * @brief add a vector value to rgb
 * @param rgb1 that is being added
 * @param rgb2 that is being added
 * @param rgb_out that has rgb1+rgb2
 * @return rgb_out that was added
 */
RGB*
rgb_add_vector(const RGB* rgb1, const RGB* rgb2, RGB* rgb_out)
{

    rgb_out->red = rgb1->red + rgb2->red;
    rgb_out->green = rgb1->green + rgb2->green;
    rgb_out->blue = rgb1->blue + rgb2->blue;

    return rgb_out;
}

/**
 * @brief multiply a vector value to rgb
 * @param rgb1 that is being multiplied
 * @param rgb2 that is being multiplied
 * @param rgb_out that has rgb1*rgb2
 * @return rgb_out that was multplied
 */
RGB*
rgb_mult_vector(const RGB* rgb1, const RGB* rgb2, RGB* rgb_out)
{
    rgb_out->red = rgb1->red * rgb2->red;
    rgb_out->green = rgb1->green * rgb2->green;
    rgb_out->blue = rgb1->blue * rgb2->blue;

    return rgb_out;
}

/**
 * @brief divide a vector value to rgb
 * @param rgb1 that is being divided
 * @param rgb2 that is being divided
 * @param rgb_out that has rgb1/rgb2
 * @return rgb_out that was divided
 */
RGB*
rgb_div_vector(const RGB* rgb1, const RGB* rgb2, RGB* rgb_out)
{
    rgb_out->red = rgb1->red / rgb2->red;
    rgb_out->green = rgb1->green / rgb2->green;
    rgb_out->blue = rgb1->blue / rgb2->blue;

    return rgb_out;
}

/**
 * @brief create a ppm image
 * @param magic_number value for the ppm image
 * @param height value for the ppm image
 * @param width value for the ppm image
 * @param max_intensity value for the ppm image
 * @reutn the created ppm image
 */
PPMImage
image_create(const char magic_number[MAGIC_NUMBER_LENGTH],
             const int height,
             const int width,
             const int max_intensity)
{
    PPMImage image;
    image.height = height;
    image.width = width;
    image.max_intensity = max_intensity;

    strncpy(image.magic_number, magic_number, MAGIC_NUMBER_LENGTH);

    image.data = (RGB**) malloc(height * sizeof(RGB*));

    for (int i = 0; i < height; i++) {
        image.data[i] = (RGB*) malloc(width * sizeof(RGB));
    };
    return image;
}

/**
 * @brief destroy a ppm image
 * @param image being destroyed
 */
void
image_destroy(PPMImage* image)
{
    for (int i = 0; i < image->height; i++) {
        free(image->data[i]);
    };
    free(image->data);
    image->data = NULL;
    image = NULL;
}

/**
 * @brief clone a ppm image
 * @param image being cloned
 * @return the cloned image
 */
PPMImage
image_clone(PPMImage* image)
{
    PPMImage out = image_create(image->magic_number,
                                image->height,
                                image->width,
                                image->max_intensity);

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            out.data[i][j] = image->data[i][j];
        }
    }
    return out;
}

/**
 * @brief copy a ppm image from src to dest
 * @param src ppm image being copied
 * @param dest ppm imaged being copied into
 * @return copied ppm image dest
 */
PPMImage*
image_copy(PPMImage* src, PPMImage* dest)
{
    if (src->height != dest->height || src->width != dest->width) {
        return NULL;
    }
    for (int i = 0; i < src->height; i++) {
        for (int j = 0; j < src->width; j++) {
            dest->data[i][j] = src->data[i][j];
        }
    }
    return dest;
}

/**
 * @brief fills the ppm image data with rgb values
 * @param image being filled
 * @param rgb the value being filled into image data
 * @return image with filled values
 */
PPMImage*
image_full(PPMImage* image, RGB* rgb)
{
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            RGB tmp = {0, 0, 0};
            rgb_copy(rgb, &tmp);
            image->data[i][j] = tmp;
        }
    }
    return image;
}

/**
 * @brief convolves the rgb at the provided point
 * @param image being convolved
 * @param kern performing the convolution
 * @param i of the point
 * @param j of the point
 * @param normalize boolean value if the convolve is normalized or not
 * @return the sum of rgb values
 */
RGB
image_convolve_at_point(PPMImage* image,
                        PPMImage* kern,
                        int i,
                        int j,
                        bool normalize)
{
    const int height_offset = kern->height / 2;
    const int width_offset = kern->width / 2;

    const int min_height = i - height_offset;
    const int max_height = i + height_offset;

    const int min_width = j - width_offset;
    const int max_width = j + width_offset;

    RGB sum = {0, 0, 0};
    RGB kern_sum = {0, 0, 0};

    for (int img_i = min_height; img_i <= max_height; img_i++) {
        for (int img_j = min_width; img_j <= max_width; img_j++) {

            bool is_valid_row = in_range(img_i, 0, image->height);
            bool is_valid_col = in_range(img_j, 0, image->width);

            if (is_valid_row && is_valid_col) {
                const int kern_i = ilinear_map(img_i,
                                               min_height,
                                               max_height,
                                               0,
                                               kern->height - 1);
                const int kern_j =
                  ilinear_map(img_j, min_width, max_width, 0, kern->width - 1);

                const RGB img_rgb = image->data[img_i][img_j];
                const RGB kern_rgb = kern->data[kern_i][kern_j];
                RGB tmp = {0, 0, 0};

                rgb_add_vector(&kern_rgb, &kern_sum, &kern_sum);

                rgb_mult_vector(&img_rgb, &kern_rgb, &tmp);
                rgb_add_vector(&sum, &tmp, &sum);
            }
        }
    }

    if (normalize) {
        rgb_div_vector(&sum, &kern_sum, &sum);
    }

    return sum;
}

/**
 * @brief convolves the image at the provided point
 * @param image being convolved
 * @param kernel performing the convolution
 * @param normalize boolean value if the convolve is normalized or not
 * @return the convolved image
 */
PPMImage*
image_convolve(PPMImage* image, PPMImage* kernel, bool normalize)
{
    PPMImage copy = image_clone(image);

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            const RGB sum =
              image_convolve_at_point(image, kernel, i, j, normalize);
            copy.data[i][j] = sum;
        }
    }

    image_copy(&copy, image);
    image_destroy(&copy);

    return image;
}

/**
 * @brief square pads the image
 * @param pad_height of the padding
 * @param pad_width of the padding
 * @param image being padded
 * @param pad_color of the padding
 * @return a padded image
 */
PPMImage
image_square_pad(const int pad_height,
                 const int pad_width,
                 PPMImage* image,
                 RGB* pad_color)
{
    if (pad_height < 0 || pad_width < 0) {
        return *image;
    }
    PPMImage new_image = image_create(image->magic_number,
                                      image->height + 2 * pad_height,
                                      image->width + 2 * pad_width,
                                      image->max_intensity);
    image_full(&new_image, pad_color);

    const int min_height = pad_height;
    const int max_height = new_image.height - pad_height;

    const int min_width = pad_width;
    const int max_width = new_image.width - pad_width;

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            int new_i =
              ilinear_map(i, 0, image->height, min_height, max_height);
            int new_j = ilinear_map(j, 0, image->width, min_width, max_width);

            new_image.data[new_i][new_j] = image->data[i][j];
        }
    }

    image_destroy(image);
    return new_image;
}

/**
 * @brief image print  the image
 * @param image being printed
 * @param fp where the image is being written
 */
void
image_print(PPMImage* image, FILE* fp)
{
    fprintf(fp,
            "%s\n%d %d\n%d\n",
            image->magic_number,
            image->width,
            image->height,
            image->max_intensity);

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            RGB rgb = image->data[i][j];
            fprintf(fp,
                    "%d %d %d ",
                    (int) rgb.red,
                    (int) rgb.green,
                    (int) rgb.blue);
        }
        fprintf(fp, "\n");
    }
}

/**
 * @brief creates a gaussian kernel
 * @param height of kernel
 * @param width of kernel
 * @param sigma used in kernel
 * @return kernel image
 */
PPMImage
image_create_gaussian_kernel(const int height,
                             const int width,
                             const double sigma)
{
    PPMImage guassian = image_create("P3", height, width, MAX_RGB);

    const double sigma_2 = (2 * sigma * sigma);

    //double sum = 0; unused

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            const double x =
              flinear_map(i, 0, height - 1, -height / HALF, height / HALF);
            const double y =
              flinear_map(j, 0, width - 1, -width / HALF, width / HALF);

            const double v = exp(-(x * x + y * y) / sigma_2) / (M_PI_LOCAL * sigma_2);

            RGB rgb = {v, v, v};
            guassian.data[i][j] = rgb;
        }
    }
    return guassian;
}

/**
 * @brief create a ppm image from an array
 * @param height of the image
 * @param width of the image
 * @return the created image
 */
PPMImage
image_from_array(const int height, const int width, double arr[height][width])
{
    PPMImage image = image_create("P3", height, width, MAX_RGB);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            const double v = arr[i][j];
            RGB rgb = {v, v, v};
            image.data[i][j] = rgb;
        }
    }
    return image;
}

/**
 * @brief read pixels
 * @param height for pixels
 * @param width for pixels
 * @param pix to read the values into
 */
void
readPixels(int height, int width, unsigned char pix[height][width][DEPTH])
{}

/**
 * @brief write image to std out
 * @param height of image
 * @param width of image
 * @param pix of the image
 */
void
writeImage(int height, int width, unsigned char pix[height][width][DEPTH])
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < DEPTH; k++) {
                printf("%d", pix[i][j][k]);
            }
            printf(" ");
        }
        printf("\n");
    };
}

/**
 * @brief checks type of image
 */
void
checkType()
{}

/**
 * @brief reads dimensions of the image
 * @return read in dimensions
 */
int
readDimension()
{
    return 0;
}

/**
 * @brief check the range of the image
 */
void
checkRange()
{}

// int
// main()
// {
// fp = fopen("tmp.ppm", "w");

// PPMImage image = parse_ppm();
// // PPMImage kernel = image_create_gaussian_kernel(7, 7, 0.84089642);

// // double arr[3][3] = {{1, 0, 0}, {0, 1, 0}, {1, 0, 1}};
// // double arr[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};

// PPMImage kernel = image_from_array(3, 3, GAUSSIAN);

// RGB black = {0, 0, 0};
// RGB white = {255, 255, 255};
// RGB one = {1, 1, 1};
// RGB red = {255, 0, 0};

// image_convolve(&image, &kernel, true);

// // PPMImage new_image = image_square_pad(100, 100, &image, &black);

// image_print(&image);
// image_destroy(&image);
// }
