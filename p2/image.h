/**
 * @file image.h
 * @author Mike Babb (mbabb),  Sanjana Cheerla (scheerl)
 * handles all functions necessary to crop/border, brighten/darken, and blur on an image
 */
#ifndef image_h
#define image_h

#include <stdbool.h>
#include <stdio.h>

/**
 * length of magic number
 */
#define MAGIC_NUMBER_LENGTH 9
/**
 * depth of pix
 */
#define DEPTH 3

/**
 * rgb struct of red green and blue values
 */
typedef struct {
    int red;
    int blue;
    int green;
} RGB;

/**
 * ppm image struuct of magic number, height, width, max_intensity, data (2 d array)
 */
typedef struct {
    char magic_number[MAGIC_NUMBER_LENGTH];
    int height;
    int width;
    int max_intensity;
    RGB** data;
} PPMImage;

/**
 * @brief clamp x between two values min and max
 * @param min clamp value
 * @param max clamp value
 * @return x if x is between min and max, max if x > max, and min if x < min
 */
int
clamp(const int x, const int min, const int max);

/**
 * @brief lerps t with min and max
 * @param min for the lerp
 * @param max for the lerp
 */
int
lerp(const int t, const int min, const int max);

/**
 * @brief checks to see if x is in between min and max
 * @param x being checked
 * @param min for the range
 * @param max for the range
 * @return true if the value x is >= min or < max
 *
 */
bool
in_range(const int x, const int min, const int max);

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
flinear_map(float t, float x1, float y1, float x2, float y2);

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
ilinear_map(int t, int x1, int y1, int x2, int y2);

/**
 * @brief return min of a and b
 * @param b being checked
 * @param a being checked
 * @return the min of a and b
 */
int
imin(int b, int a);

/**
 * @brief return max of a and b
 * @param b being checked
 * @param a being checked
 * @return the max of a and b
 */
int
imax(int b, int a);

/**
 * @brief clamp rgb values using clamp method
 * @param rgb being clamped
 * @param min for clamp
 * @param max for clamp
 * @return rgb that was clamped
 */
RGB*
rgb_clamp(RGB* rgb, int min, int max);

/**
 * @brief add a scalar value to rgb
 * @param x being added
 * @param rgb that x is being added to
 * @return rgb that was added
 */
RGB*
rgb_add_scalar(int x, RGB* rgb);

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
             const int max_intensity);

/**
 * @bried destroy a ppm image
 * @param image being destroyed
 */
void
image_destroy(PPMImage* image);

/**
 * @brief fills the ppm image data with rgb values
 * @param image being filled
 * @param rgb the value being filled into image data
 * @return image with filled values
 */
PPMImage*
image_full(PPMImage* image, RGB* rgb);

/**
 * @brief convolves the image at the provided point
 * @param image being convolved
 * @param kernel performing the convolution
 * @param normalize boolean value if the convolve is normalized or not
 * @return the convolved image
 */
PPMImage*
image_convolve(PPMImage* image, PPMImage* kernel, bool normalize);

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
                 RGB* pad_color);

/**
 * @brief image print  the image
 * @param image being printed
 * @param fp where the image is being written
 */
void
image_print(PPMImage* image, FILE* fp);

/**
 * @brief create a ppm image from an array
 * @param height of the image
 * @param width of the image
 * @return the created image
 */
PPMImage
image_from_array(const int height, const int width, double arr[height][width]);


/**
 * @brief read pixels
 * @param height for pixels
 * @param width for pixels
 * @param pix to read the values into
 */
void
readPixels(int height, int width, unsigned char pix[height][width][DEPTH]);

/**
 * @brief write image to std out
 * @param height of image
 * @param width of image
 * @param pix of the image
 */
void
writeImage(int height, int width, unsigned char pix[height][width][DEPTH]);

/**
 * @brief checks type of image
 */
void
checkType();

/**
 * @brief reads dimensions of the image
 * @return read in dimensions
 */
int
readDimension();

/**
 * @brief check the range of the image
 */
void
checkRange();

#endif
