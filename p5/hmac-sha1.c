/**
 * @file hmac-sha1.h
 * @author Mike Babb
 * @author Sanjana Cheerla
 * @brief compute hmac
 */
#include "hmac-sha1.h"

#define IPAD 0x36
#define OPAD 0x5C

#define PAD 64

/**
 * @brief perform the hmac operation
 * @param kstr message
 * @param b buffer
 * @param digest for sha1
 */
void
hmacSHA1(char* key, Buffer* buff, unsigned char digest[SHA1_DIGEST])
{
    Buffer* key_buff = read_str(key);

    if (key_buff->len > SHA1_BLOCK) {
        sha1_hash(key_buff);
    }

    if (key_buff->len < SHA1_BLOCK) {
        size_t pad_amount = SHA1_BLOCK - key_buff->len;
        pad_buffer_n(key_buff, 0, pad_amount);
    }

    Buffer* key_buff_2 = makeBuffer();
    to_buffer(key_buff_2, key_buff->data, SHA1_BLOCK);

    for (int i = 0; i < PAD; i++) {
        key_buff->data[i] ^= IPAD;
        key_buff_2->data[i] ^= OPAD;
    }

    concat_buffer(key_buff, buff);
    sha1_hash_digest(key_buff, digest);

    Buffer* right_buff = makeBuffer();
    to_buffer(right_buff, digest, SHA1_DIGEST);

    concat_buffer(key_buff_2, right_buff);
    sha1_hash_digest(key_buff_2, digest);

    char char_digest[SHA1_BLOCK] = {0};
    char* hash = unsigned_char_to_hex(char_digest, digest, SHA1_DIGEST);

    printf("%s\n", hash);

    freeBuffer(key_buff);
    freeBuffer(key_buff_2);
    freeBuffer(right_buff);
}
