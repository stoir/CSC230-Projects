/**
 * @file hmac-sha1.h
 * @author Mike Babb
 * @author Sanjana Cheerla
 * @brief compute hmac
 */
#ifndef _HMAC_SHA1_H_
#define _HMAC_SHA1_H_

#include "buffer.h"
#include "sha1.h"

/**
 * @brief perform the hmac operation
 * @param kstr message
 * @param b buffer
 * @param digest for sha1
 */
void
hmacSHA1(char* kstr, Buffer* b, unsigned char digest[SHA1_DIGEST]);

#endif
