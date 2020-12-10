/**
 * @file sha1.c
 * @author Mike Babb
 * @author Sanjana Cheerla
 * @brief compute sha1
 */

#include "sha1.h"

#include "buffer.h"
#include "vector/vector.h"

#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define ITERATIONS 80
#define PRINT_VAL 2
#define THIRTY_TWO_BIT 32
#define SIXTY_FOUR_BIT 64
#define SIXTEEN_BIT 16
#define ROTATE 5
#define BUFF_SZ 8
#define ROT_L 30
#define W_MIN 3
#define W_MAX 14
#define F_ZERO_MAX 19
#define F_ONE_MAX 39
#define F_TWO_MAX 59
#define F_THREE_MAX 79
#define PAD_AMT 56
#define APPEND_BUFFER_VAL 0x80

/** Constants, to mix in some random-looking bits during the SHA1
    calculation.  80 constants for 80 iterations.  The constants for one
    round (iterations 0-19, 20-39, 40-59, 60-79) all have the same value. **/
static uint32_t k[ITERATIONS] = {
  0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999,
  0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999,
  0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999, 0x5A827999,
  0x5A827999, 0x5A827999, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1,
  0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1,
  0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1,
  0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x6ED9EBA1, 0x8F1BBCDC, 0x8F1BBCDC,
  0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC,
  0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC,
  0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC, 0x8F1BBCDC,
  0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6,
  0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6,
  0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6, 0xCA62C1D6,
  0xCA62C1D6, 0xCA62C1D6,
};

#define swap(x, y)                                                             \
    do {                                                                       \
        unsigned char                                                          \
          swap_temp[sizeof(x) == sizeof(y) ? (signed) sizeof(x) : -1];         \
        memcpy(swap_temp, &y, sizeof(x));                                      \
        memcpy(&y, &x, sizeof(x));                                             \
        memcpy(&x, swap_temp, sizeof(x));                                      \
    } while (0)

/**
 * @brief convert val to big endian
 * @param val to convert
 */
void
little_to_big_endian(uint32_t* val)
{
    char* p = (char*) val;
    swap(p[0], p[PRINT_VAL + 1]);
    swap(p[1], p[PRINT_VAL]);
}

/**
 * @brief rotate x right by y
 * @param x to rotate
 * @param y amount to rotate
 */
test_static uint32_t
rotateRight(uint32_t x, uint32_t y)
{
    // 11111 000
    //     11111 ->> 3
    // 000 00000 <<- (sizeof(x) - 3)
    // 000 11111
    uint32_t shift = THIRTY_TWO_BIT - y;
    return (x << shift) | (x >> y);
}

/**
 * @brief rotate x left by y
 * @param x to rotate
 * @param y amount to rotate
 */
test_static uint32_t
rotateLeft(uint32_t x, uint32_t y)
{
    uint32_t shift = THIRTY_TWO_BIT - y;
    return (x << y) | (x >> (shift));
}

/**
 * @brief initialize state
 */
void
initState(SHA1State* state)
{
    state->h0 = 0x67452301U;
    state->h1 = 0xEFCDAB89U;
    state->h2 = 0x98BADCFEU;
    state->h3 = 0x10325476U;
    state->h4 = 0xC3D2E1F0U;
}

/**
 * @brief append buffer
 * @param buff to append to
 * @param ch to append
 * @param element_size of ch
 */
void
append_buffer(Buffer* buff, const void* ch, const size_t element_size)
{
    char* tmp = (char*) ch;
    for (int i = 0; i < element_size; i++) {
        appendBuffer(buff, tmp[i]);
    }
}

void
to_buffer(Buffer* buff, unsigned char* arr, size_t n)
{
    for (int i = 0; i < n; i++) {
        unsigned char ch = arr[i];
        appendBuffer(buff, ch);
    }
}

/**
 * @brief pad buffer with values
 * @param buff to pad
 * @param value to pad with
 * @param pad_amount to pad
 */
void
pad_buffer_n(Buffer* buff, unsigned char value, size_t pad_amount)
{
    for (size_t i = 0; i < pad_amount; i++) {
        appendBuffer(buff, value);
    }
}

/**
 * @brief concatinate buff2 to buff 1
 * @param buff1 to concatinate to
 * @param buff2 to concatinate from
 */
void
concat_buffer(Buffer* buff1, Buffer* buff2)
{
    for (int i = 0; i < buff2->len; i++) {
        appendBuffer(buff1, buff2->data[i]);
    }
}

/**
 * @brief pad buffer
 * @param buff buffer
 */
void
padBuffer(Buffer* buff)
{

    size_t size = buff->data_vector.size;
    size_t pad_amount = ((PAD_AMT - (size + 1) % SIXTY_FOUR_BIT) % SIXTY_FOUR_BIT); 

    appendBuffer(buff, (unsigned char) APPEND_BUFFER_VAL);

    pad_buffer_n(buff, (unsigned char) 0x00, pad_amount);

    size *= BUFF_SZ;
    for (int i = BUFF_SZ- 1; i >= 0; i--) {
        unsigned char ch = ((char*) &size)[i];
        appendBuffer(buff, ch);
    }
}

/**
 * @brief perform f version 0
 * @param b used in f
 * @param c used in f
 * @param d used in f
 */
test_static uint32_t
fVersion0(uint32_t b, uint32_t c, uint32_t d)
{
    return (b & c) | ((~b) & d);
}

/**
 * @brief perform f version 1
 * @param b used in f
 * @param c used in f
 * @param d used in f
 */
test_static uint32_t
fVersion1(uint32_t b, uint32_t c, uint32_t d)
{
    return b ^ c ^ d;
}

/**
 * @brief perform f version 2
 * @param b used in f
 * @param c used in f
 * @param d used in f
 */
test_static uint32_t
fVersion2(uint32_t b, uint32_t c, uint32_t d)
{
    return (b & c) | (b & d) | (c & d);
}

/**
 * @brief perform f version 3
 * @param b used in f
 * @param c used in f
 * @param d used in f
 */
test_static uint32_t
fVersion3(uint32_t b, uint32_t c, uint32_t d)
{
    return b ^ c ^ d;
}

/**
 * @brief select an f value and perform the f functions based on the version
 * @param i to select an f
 * @param b used in f
 * @param c used in f
 * @param d used in f
 */
uint32_t
f(int i, uint32_t b, uint32_t c, uint32_t d)
{
    if (i <= 0 || i <= F_ZERO_MAX) {
        return fVersion0(b, c, d);
    } else if (i <= F_ZERO_MAX + 1 || i <= F_ONE_MAX) {
        return fVersion1(b, c, d);
    } else if (i <= F_ONE_MAX + 1 || i <= F_TWO_MAX) {
        return fVersion2(b, c, d);
    } else if (i <= F_TWO_MAX + 1 || i <= F_THREE_MAX) {
        return fVersion3(b, c, d);
    } else {
        return 0;
    }
}

/**
 * @brief performs one iteration of sha1
 * @param data of iteration
 * @param a used in iteration
 * @param b used in iteration
 * @param c used in iteration
 * @param d used in iteration
 * @param e used in iteration
 * @param i used in f
 */
test_static void
sha1Iteration(uint32_t data[SIXTEEN_BIT],
              uint32_t* a,
              uint32_t* b,
              uint32_t* c,
              uint32_t* d,
              uint32_t* e,
              int i)
{
    uint32_t w[ITERATIONS] = {};
    memcpy(w, data, SIXTY_FOUR_BIT);

    for (int j = SIXTEEN_BIT; j < ITERATIONS; j++) {
        w[j] = rotateLeft(w[j - W_MIN] ^ w[j - BUFF_SZ] ^ w[j - W_MAX] ^ w[j - SIXTEEN_BIT], 1);
    }

    uint32_t w_i = w[i];
    uint32_t k_i = k[i];
    uint32_t rotl_i = rotateLeft(*a, ROTATE);
    uint32_t f_i = f(i, *b, *c, *d);

    uint32_t tmp = rotl_i + f_i + *e + k_i + w_i;

    *e = *d;
    *d = *c;
    *c = rotateLeft(*b, ROT_L);
    *b = *a;
    *a = tmp;
}

/**
 * @brief sha1 block performs 80 SHA1 iterations on the given block of bytes,
 * updating the given state.
 * @param data block array
 * @param state for sha1
 */
void
sha1Block(unsigned char data[SHA1_BLOCK], SHA1State* state)
{

    uint32_t a = state->h0;
    uint32_t b = state->h1;
    uint32_t c = state->h2;
    uint32_t d = state->h3;
    uint32_t e = state->h4;

    uint32_t w[SIXTEEN_BIT] = {};
    memcpy(w, data, SHA1_BLOCK);

#ifndef TESTABLE
    for (int i = 0; i < SIXTEEN_BIT; i++) {
        little_to_big_endian(&w[i]);
    }
#endif

    for (int i = 0; i < ITERATIONS; i++) {
        sha1Iteration(w, &a, &b, &c, &d, &e, i);
    }

    state->h0 += a;
    state->h1 += b;
    state->h2 += c;
    state->h3 += d;
    state->h4 += e;
}

/**
 * @brief create final sha1 digest
 * @param digest array
 * @param state for sha1
 */
void
sha1Encode(unsigned char digest[SHA1_DIGEST], SHA1State* state)
{
    uint32_t tmp[] = {state->h0, state->h1, state->h2, state->h3, state->h4};

#ifndef TESTABLE
    for (int i = 0; i < ROTATE; i++) {
        little_to_big_endian(&tmp[i]);
    }
#endif

    memcpy(digest, tmp, SHA1_DIGEST);
}

/**
 * @brief break sha1 into a size 64 chunk
 * @param buff buffer
 * @param state for sha1
 */
void
sha1_chunk64(Buffer* buff, SHA1State* state)
{
    const size_t n = buff->len / SHA1_BLOCK;
    unsigned char* block = buff->data;

    for (int i = 0; i < n; i++) {
        unsigned char* t_block = &block[i * SHA1_BLOCK];
        sha1Block(t_block, state);
    }
}

/**
 * @brief pads buffer, and performs sha1_chunk64 on state
 * @param buff buffer
 */
SHA1State
sha1_hash(Buffer* buff)
{
    padBuffer(buff);

    SHA1State state;
    initState(&state);

    sha1_chunk64(buff, &state);
    return state;
}

void
sha1_hash_digest(Buffer* buff, unsigned char digest[SHA1_DIGEST])
{
    SHA1State state = sha1_hash(buff);
    sha1Encode(digest, &state);
}

/**
 * @brief convert unsigned char to hex
 * @param dest destination hex
 * @param src source unsigned char
 * @param size size
 */
char*
unsigned_char_to_hex(char* dest, unsigned char* src, size_t size)
{
    for (int i = 0; i < size; i++) {
        unsigned char ch = src[i];
        sprintf(dest + PRINT_VAL * i, "%02X", ch);
    }
    return dest;
}
