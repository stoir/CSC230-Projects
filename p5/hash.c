/**
 * @file hash.c
 * @author Mike Babb
 * @author Sanjana Cheerla
 * @brief compute sha1
 */

#include "hmac-sha1.h"
#include "sha1.h"

#define HMAC 4
#define SHA1 2

/**
 * @brief main method for hash, create a sha1 hash or a hmac hash
 * @param argc number of arguments
 * @param argv arguments
 */
int
main(int argc, char* argv[])
{
    if (argc != HMAC && argc != SHA1) { //4 2
        fprintf(stderr, "usage: hash [-hmac <key>] <filename>\n");
        exit(EXIT_FAILURE);
    }

    if (argc == HMAC && strcmp(argv[1], "-hmac") != 0) {
        fprintf(stderr, "usage: hash [-hmac <key>] <filename>\n");
        exit(EXIT_FAILURE);
    }

    int file_idx = argc == SHA1 ? 1 : HMAC - 1; //3

    const char* filename = argv[file_idx];
    Buffer* buff = readFile(filename);

    if (buff == NULL) {
        fprintf(stderr, "Can't open file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    unsigned char digest[SHA1_DIGEST] = {0};

    if (argc == HMAC) {
        char* key = argv[SHA1];
        hmacSHA1(key, buff, digest);
    } else {
        sha1_hash_digest(buff, digest);

        char char_digest[SHA1_BLOCK] = {0};
        char* hash = unsigned_char_to_hex(char_digest, digest, SHA1_DIGEST);

        printf("%s\n", hash);
    }

    freeBuffer(buff);

    return EXIT_SUCCESS;
}
