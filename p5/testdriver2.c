/**
    @file testdriver.c

    This is a test driver for code in the buffer and sha1 components.
*/

#include "buffer.h"
#include "sha1.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Total number or tests we tried. */
static int totalTests = 0;

/** Number of test cases passed. */
static int passedTests = 0;

/** Macro to check the condition on a test case, keep counts of passed/failed
   tests and report a message if the test fails. */
#define TestCase(conditional)                                                  \
    {                                                                          \
        totalTests += 1;                                                       \
        if (conditional) {                                                     \
            passedTests += 1;                                                  \
        } else {                                                               \
            printf("**** Failed unit test on line %d of %s\n",                 \
                   __LINE__,                                                   \
                   __FILE__);                                                  \
        }                                                                      \
    }

// These functions aren't really part of the public interface for the sha1
// component, but, to test them we compile without marking them as static.
// Then, they're visible from other components, but we still need a prototype
// if we're going to be able to call them.
void
sha1Iteration(unsigned int wp[16],
              unsigned int* aptr,
              unsigned int* bptr,
              unsigned int* cptr,
              unsigned int* dptr,
              unsigned int* eptr,
              int i);

int
main()
{
    // As you finish parts of your implementation, move this directive
    // down past the blocks of code below.  That will enable tests of
    // various functions you're expected to implement.

    // Here's an extr test for the sha1Interation() function in the last
    // round.  The original test driver didnt test this round.
    {
        unsigned int data[] = {0XD00BC311U,
                               0X9D78FC11U,
                               0X9DFC267CU,
                               0X28CAE49CU,
                               0X38BF97CCU,
                               0X9BC65212U,
                               0X90C3DE78U,
                               0X38E80CC9U,
                               0XDCDF79D6U,
                               0XE39268AEU,
                               0X4333A8DBU,
                               0X7B4DEDEDU,
                               0X508BB3F9U,
                               0XFE684444U,
                               0XDF72383FU,
                               0X2332CB00U};

        unsigned int a = 0X637B4243;
        unsigned int b = 0X64318146;
        unsigned int c = 0X8EE138CD;
        unsigned int d = 0X621D9909;
        unsigned int e = 0X40D00518;
        sha1Iteration(data, &a, &b, &c, &d, &e, 67);

        TestCase(a == 0x45D2186F);
        TestCase(b == 0x637B4243);
        TestCase(c == 0x990C6051);
        TestCase(d == 0x8EE138CD);
        TestCase(e == 0x621D9909);
    }

    // Here's a test that tries to simulate what should happen on test input
    // input-1.txt
    {
        SHA1State state;
        initState(&state);

        // Here's what input-1.txt should look like after you've read it
        // into a buffer, Added padding at the end and reordered the bytes
        // of each word to match the byte order on the common platform.
        unsigned char data[64] = {0x73, 0x69, 0x68, 0x54, 0x20, 0x73, 0x69,
                                  0x20, 0x68, 0x73, 0x20, 0x61, 0x20, 0x74,
                                  0x72, 0x6F, 0x75, 0x70, 0x6E, 0x69, 0x69,
                                  0x66, 0x20, 0x74, 0x0A, 0x2E, 0x65, 0x6C,
                                  0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                  0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00,
                                  0x00};

        // We just have one block for this first input file.
        sha1Block(data, &state);

        // Check to make sure we hae the right state when we're done.
        TestCase(state.h0 == 0x6B0BAF83);
        TestCase(state.h1 == 0x7E4CC8D2);
        TestCase(state.h2 == 0x06D65FB5);
        TestCase(state.h3 == 0x9A00A706);
        TestCase(state.h4 == 0x9C392D2E);

        // Turn the state into a sequence of bytes (in MSB-first order.
        unsigned char digest[20];
        sha1Encode(digest, &state);

        TestCase(digest[0] == 0x6B);
        TestCase(digest[1] == 0x0B);
        TestCase(digest[2] == 0xAF);
        TestCase(digest[3] == 0x83);
        TestCase(digest[4] == 0x7E);
        TestCase(digest[5] == 0x4C);
        TestCase(digest[6] == 0xC8);
        TestCase(digest[7] == 0xD2);
        TestCase(digest[8] == 0x06);
        TestCase(digest[9] == 0xD6);
        TestCase(digest[10] == 0x5F);
        TestCase(digest[11] == 0xB5);
        TestCase(digest[12] == 0x9A);
        TestCase(digest[13] == 0x00);
        TestCase(digest[14] == 0xA7);
        TestCase(digest[15] == 0x06);
        TestCase(digest[16] == 0x9C);
        TestCase(digest[17] == 0x39);
        TestCase(digest[18] == 0x2D);
        TestCase(digest[19] == 0x2E);
    }

    printf("You passed %d / %d unit tests\n", passedTests, totalTests);

    if (passedTests != 30)
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;
}
