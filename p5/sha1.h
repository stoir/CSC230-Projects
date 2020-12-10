/**
 * @file sha1.h
 * @author Mike Babb
 * @author Sanjana Cheerla
 * @brief compute sha1
 */

#ifndef _SHA1_H_
#define _SHA1_H_

#include "buffer.h"

/* Mechanism to conditionally expose static functions to other components.  For
   production, we can make make them static, but for testing we can disable
   the static keyword and expose functions to the test driver. */
// #ifdef TESTABLE
// #define test_static
// #else
// #define test_static static
// #endif
#define test_static
/** Number of bytes in a block used in the SHA1 calculation. */
#define SHA1_BLOCK 64ul

/** Number of bytes in an SHA1 digest */
#define SHA1_DIGEST 20ul

/** Representation for the state of the SHA1 computation.  It's just 5
    unsigned 32-bit integers. Client code can create an instance
    (statically, on the stack or on the heap), but initState() needs
    to initialize it before it can be used. */
typedef struct {
    /** Fields h0 through h4 of the SHA1 state.  */
    unsigned int h0;
    unsigned int h1;
    unsigned int h2;
    unsigned int h3;
    unsigned int h4;
} SHA1State;

/**
 * @brief initialize state
 */
void
initState(SHA1State* state);

/**
 * @brief append buffer
 * @param buff to append to
 * @param ch to append
 * @param element_size of ch
 */
void
append_buffer(Buffer* buff, const void* ch, const size_t element_size);

/**
 * @brief concatinate buff2 to buff 1
 * @param buff1 to concatinate to
 * @param buff2 to concatinate from
 */
void concat_buffer(Buffer* buff1, Buffer* buff2);

/**
 * @brief pad buffer with values
 * @param buff to pad
 * @param value to pad with
 * @param pad_amount to pad
 */
void
pad_buffer_n(Buffer* buff, unsigned char value, size_t pad_amount);

/**
 * @brief pad buffer
 * @param b buffer
 */
void
padBuffer(Buffer* b);

/**
 * @brief sha1 block performs 80 SHA1 iterations on the given
 * block of bytes, updating the given state.
 * @param data block array
 * @param state for sha1
 */
void
sha1Block(unsigned char data[SHA1_BLOCK], SHA1State* state);

/**
 * @brief create final sha1 digest
 * @param digest array
 * @param state for sha1
 */
void
sha1Encode(unsigned char digest[SHA1_DIGEST], SHA1State* state);

/**
 * @brief break sha1 into a size 64 chunk
 * @param buff buffer
 * @param state for sha1
 */
void
sha1_chunk64(Buffer* buff, SHA1State* state);

/**
 * @brief pads buffer, and performs sha1_chunk64 on state
 * @param buff buffer
 */
SHA1State
sha1_hash(Buffer* buff);

/**
 * @brief convert unsigned char to hex
 * @param dest destination hex
 * @param src source unsigned char
 * @param size size
 */
char*
unsigned_char_to_hex(char* dest, unsigned char* src, size_t size);

void
to_buffer(Buffer* buff, unsigned char* arr, size_t n);

void
sha1_hash_digest(Buffer* buff, unsigned char digest[SHA1_DIGEST]);

#endif
