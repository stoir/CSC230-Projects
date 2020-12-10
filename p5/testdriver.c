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
unsigned int
fVersion0(unsigned int b, unsigned int c, unsigned int d);
unsigned int
fVersion1(unsigned int b, unsigned int c, unsigned int d);
unsigned int
fVersion2(unsigned int b, unsigned int c, unsigned int d);
unsigned int
fVersion3(unsigned int b, unsigned int c, unsigned int d);
unsigned int
rotateLeft(unsigned int value, int s);
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
    // As you finish parts of your implementation, move this directive down past
    // the blocks of code below.  That will enable tests of various functions
    // you're expected to implement.

    // Tests for buffer component.

    // Test makeBuffer()
    {
        Buffer* b = makeBuffer();

        TestCase(b != NULL);
        TestCase(b->len == 0);
        TestCase(b->cap == 64);

        freeBuffer(b);
    }

    // Test appendBuffer()
    {
        Buffer* b = makeBuffer();

        // Insert characters and make sure the buffer gets updated.
        appendBuffer(b, 'v');
        TestCase(b->len == 1);
        TestCase(b->data[0] == 'v');

        appendBuffer(b, 'w');
        TestCase(b->len == 2);
        TestCase(b->data[1] == 'w');

        appendBuffer(b, 'x');
        TestCase(b->len == 3);
        TestCase(b->data[2] == 'x');

        appendBuffer(b, 'y');
        TestCase(b->len == 4);
        TestCase(b->data[3] == 'y');

        // Shouldn't have needed to grow yet.
        TestCase(b->cap == 64);

        // Try adding a bunch of characters.
        for (int i = 0; i < 60; i++)
            appendBuffer(b, 'z');
        TestCase(b->len == 64);
        TestCase(b->data[63] == 'z');
        TestCase(b->cap >= 64);

        // One more character should force the buffer to grow.
        appendBuffer(b, '!');

        TestCase(b->len == 65);
        TestCase(b->cap >= 128);
        TestCase(b->data[64] == '!');

        freeBuffer(b);
    }

    // Test readFile()
    {{Buffer* b = readFile("input-1.txt");
    TestCase(b != NULL);

    TestCase(b->len == 28);
    // Make sure the buffer contains the right sequence of bytes.
    TestCase(memcmp(b->data, "This is a short input file.\n", 28) == 0);
    TestCase(b->cap >= 28);

    freeBuffer(b);
}

{
    Buffer* b = readFile("input-3.txt");

    // Make sure it looks like we got the whole input file.
    TestCase(b->len == 909);
    TestCase(b->data[907] == '.');
    TestCase(b->cap >= 909);

    freeBuffer(b);
}

{
    // Try the binary input file.
    Buffer* b = readFile("input-5.bin");

    // Make sure it looks like we got the whole input file.
    TestCase(b->len == 11328);
    TestCase(b->data[11327] == 0x1b);
    TestCase(b->cap >= 11328);

    freeBuffer(b);
}

{
    // Try a file that doesn't exist.
    Buffer* b = readFile("no-input-file.txt");
    TestCase(b == NULL);
}
}

// Tests for the sha1 component

// Test the four versions of the F function;
{{unsigned int b = 0x00000001;
unsigned int c = 0x00000003;
unsigned int d = 0x00000007;
unsigned int result;

result = fVersion0(b, c, d);
TestCase(result == 0x00000007);

result = fVersion1(b, c, d);
TestCase(result == 0x00000005);

result = fVersion2(b, c, d);
TestCase(result == 0x00000003);

result = fVersion3(b, c, d);
TestCase(result == 0x00000005);
}

{
    unsigned int b = 0xAB659F92;
    unsigned int c = 0xE4C6A120;
    unsigned int d = 0xC0D29093;
    unsigned int result;

    result = fVersion0(b, c, d);
    TestCase(result == 0xE0D68101);

    result = fVersion1(b, c, d);
    TestCase(result == 0x8F71AE21);

    result = fVersion2(b, c, d);
    TestCase(result == 0xE0C69192);

    result = fVersion3(b, c, d);
    TestCase(result == 0x8F71AE21);
}
}

// Test the rotateLeft() function.
{
    unsigned int result;

    result = rotateLeft(0x00000001, 1);
    TestCase(result == 0x00000002);

    result = rotateLeft(0x80000000, 1);
    TestCase(result == 0x00000001);

    result = rotateLeft(0x6223818F, 3);
    TestCase(result == 0x111C0C7B);

    result = rotateLeft(0x9C913C27, 11);
    TestCase(result == 0x89E13CE4);
}

// void initState( SHA1State *state );

// Test the initState() function.
{
    SHA1State state;

    initState(&state);
    TestCase(state.h0 == 0x67452301);
    TestCase(state.h1 == 0xEFCDAB89);
    TestCase(state.h2 == 0x98BADCFE);
    TestCase(state.h3 == 0x10325476);
    TestCase(state.h4 == 0xC3D2E1F0);
}

// Test the padBuffer() function.
{{// Make a buffer and put some characters into it.
  Buffer* b = makeBuffer();

char* str = "testing 1, 2, 3...";
for (int i = 0; str[i]; i++)
    appendBuffer(b, str[i]);
padBuffer(b);

TestCase(b->len == 64);

// This should be the buffer, padded with a 0x80, then enough zero bytes
// to make it 8 bytes less than a multiple of 64 bytes in length.
// Then, the last 8 bytes are the number of bits in the original
// message, written out in MSB-First order.
unsigned char expected[64] = {0x74, 0x65, 0x73, 0x74, 0x69, 0x6E, 0x67, 0x20,
                              0x31, 0x2C, 0x20, 0x32, 0x2C, 0x20, 0x33, 0x2E,
                              0x2E, 0x2E, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90};
TestCase(memcmp(b->data, expected, 64) == 0);
freeBuffer(b);
}

{
    Buffer* b = makeBuffer();

    // Put some characters in the buffer.
    char* str =
      "This is a longer test string, chosen to be exactly 64 bytes long";
    for (int i = 0; str[i]; i++)
        appendBuffer(b, str[i]);
    padBuffer(b);

    TestCase(b->len == 128);

    // Same as above, but here the message gets padded to fill two 64-byte
    // blocks.
    unsigned char expected[128] =
      {0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x61, 0x20, 0x6C, 0x6F,
       0x6E, 0x67, 0x65, 0x72, 0x20, 0x74, 0x65, 0x73, 0x74, 0x20, 0x73, 0x74,
       0x72, 0x69, 0x6E, 0x67, 0x2C, 0x20, 0x63, 0x68, 0x6F, 0x73, 0x65, 0x6E,
       0x20, 0x74, 0x6F, 0x20, 0x62, 0x65, 0x20, 0x65, 0x78, 0x61, 0x63, 0x74,
       0x6C, 0x79, 0x20, 0x36, 0x34, 0x20, 0x62, 0x79, 0x74, 0x65, 0x73, 0x20,
       0x6C, 0x6F, 0x6E, 0x67, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00};

    TestCase(memcmp(b->data, expected, 128) == 0);
    freeBuffer(b);
}

{
    Buffer* b = makeBuffer();

    // Put some characters in the buffer.
    char* str =
      "This test is even longer.  It want to make it at least two "
      "blocks long, and, with padding, I'd like it to take three. "
      "Am I going to have to think of something else to say?  Guess not.";
    for (int i = 0; str[i]; i++)
        appendBuffer(b, str[i]);
    padBuffer(b);

    TestCase(b->len == 192);

    // Same as before, but now the message is a little bit longer.
    unsigned char expected[192] =
      {0x54, 0x68, 0x69, 0x73, 0x20, 0x74, 0x65, 0x73, 0x74, 0x20, 0x69, 0x73,
       0x20, 0x65, 0x76, 0x65, 0x6E, 0x20, 0x6C, 0x6F, 0x6E, 0x67, 0x65, 0x72,
       0x2E, 0x20, 0x20, 0x49, 0x74, 0x20, 0x77, 0x61, 0x6E, 0x74, 0x20, 0x74,
       0x6F, 0x20, 0x6D, 0x61, 0x6B, 0x65, 0x20, 0x69, 0x74, 0x20, 0x61, 0x74,
       0x20, 0x6C, 0x65, 0x61, 0x73, 0x74, 0x20, 0x74, 0x77, 0x6F, 0x20, 0x62,
       0x6C, 0x6F, 0x63, 0x6B, 0x73, 0x20, 0x6C, 0x6F, 0x6E, 0x67, 0x2C, 0x20,
       0x61, 0x6E, 0x64, 0x2C, 0x20, 0x77, 0x69, 0x74, 0x68, 0x20, 0x70, 0x61,
       0x64, 0x64, 0x69, 0x6E, 0x67, 0x2C, 0x20, 0x49, 0x27, 0x64, 0x20, 0x6C,
       0x69, 0x6B, 0x65, 0x20, 0x69, 0x74, 0x20, 0x74, 0x6F, 0x20, 0x74, 0x61,
       0x6B, 0x65, 0x20, 0x74, 0x68, 0x72, 0x65, 0x65, 0x2E, 0x20, 0x41, 0x6D,
       0x20, 0x49, 0x20, 0x67, 0x6F, 0x69, 0x6E, 0x67, 0x20, 0x74, 0x6F, 0x20,
       0x68, 0x61, 0x76, 0x65, 0x20, 0x74, 0x6F, 0x20, 0x74, 0x68, 0x69, 0x6E,
       0x6B, 0x20, 0x6F, 0x66, 0x20, 0x73, 0x6F, 0x6D, 0x65, 0x74, 0x68, 0x69,
       0x6E, 0x67, 0x20, 0x65, 0x6C, 0x73, 0x65, 0x20, 0x74, 0x6F, 0x20, 0x73,
       0x61, 0x79, 0x3F, 0x20, 0x20, 0x47, 0x75, 0x65, 0x73, 0x73, 0x20, 0x6E,
       0x6F, 0x74, 0x2E, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0xB8};

    TestCase(memcmp(b->data, expected, 192) == 0);
    freeBuffer(b);
}
}

// Test the sha1Iteration() function, a test for each round.
{{// Random-looking values for a data block.
  unsigned int data[] = {0x3E89FFA4U,
                         0x56AF8963U,
                         0x72B214EFU,
                         0x6E078ACEU,
                         0x539FAB27U,
                         0x754D1F0AU,
                         0xBC496D95U,
                         0x11695FEFU,
                         0xBA9ED1AAU,
                         0x192B3715U,
                         0x88D80898U,
                         0xAEE9F73EU,
                         0x02F429EDU,
                         0x7E840F0BU,
                         0x498B4509U,
                         0xFA54CF37U};
unsigned int a = 0X8C91FCE1U;
unsigned int b = 0XA3DFC292U;
unsigned int c = 0X1247C589U;
unsigned int d = 0X5403C3DCU;
unsigned int e = 0X729AB39FU;
sha1Iteration(data, &a, &b, &c, &d, &e, 0);

TestCase(a == 0xF42E8AD9U);
TestCase(b == 0x8C91FCE1U);
TestCase(c == 0xA8F7F0A4U);
TestCase(d == 0x1247C589U);
TestCase(e == 0x5403C3DCU);
}

{
    unsigned int data[] = {0x7D7B9A46U,
                           0xB7851D52U,
                           0xECCDFCF2U,
                           0x4F0192C4U,
                           0xB4587C78U,
                           0x149D9A7BU,
                           0xF24026F6U,
                           0x867C8D4FU,
                           0x044D387CU,
                           0xBEEC6302U,
                           0x3103F912U,
                           0xEB240372U,
                           0x688E8689U,
                           0x86566ADFU,
                           0xD4D78755U,
                           0x3ABE596BU};
    unsigned int a = 0x36F3E5FEU;
    unsigned int b = 0xF7CC2E7DU;
    unsigned int c = 0xC6CF8729U;
    unsigned int d = 0xF369EB5CU;
    unsigned int e = 0x815576B4U;
    sha1Iteration(data, &a, &b, &c, &d, &e, 21);

    TestCase(a == 0xF6E4DC3EU);
    TestCase(b == 0x36F3E5FEU);
    TestCase(c == 0x7DF30B9FU);
    TestCase(d == 0xC6CF8729U);
    TestCase(e == 0xF369EB5CU);
}

{
    unsigned int data[] = {0x7998A7E9U,
                           0xD67277FFU,
                           0xB6CFE5D6U,
                           0x42B44CAEU,
                           0xA0B8C114U,
                           0x606240D3U,
                           0xAF64D1C9U,
                           0x51E7D5CAU,
                           0x2CD988C3U,
                           0x647164DAU,
                           0xB42C169AU,
                           0x27710515U,
                           0xD442D6F8U,
                           0xC51BA1A9U,
                           0x54ECBB60U,
                           0xC34B15D8U};
    unsigned int a = 0x67F629DFU;
    unsigned int b = 0x8E302042U;
    unsigned int c = 0x8C198D10U;
    unsigned int d = 0x9ADE4754U;
    unsigned int e = 0xBE240EB3U;
    sha1Iteration(data, &a, &b, &c, &d, &e, 38);

    TestCase(a == 0x3AA0F0ADU);
    TestCase(b == 0x67F629DFU);
    TestCase(c == 0xA38C0810U);
    TestCase(d == 0x8C198D10U);
    TestCase(e == 0x9ADE4754U);
}

{
    unsigned int data[] = {0xEA4902D6U,
                           0x6B7E4628U,
                           0xAAA617A2U,
                           0xE1D86531U,
                           0x09107730U,
                           0xE98F9D4BU,
                           0x3ACC9173U,
                           0xC006C27AU,
                           0xB9B3044DU,
                           0x7F809884U,
                           0x62D9856DU,
                           0xCEB1E48AU,
                           0xC1B33EA0U,
                           0x45AFC5EEU,
                           0x5C22A633U,
                           0x944E0062U};
    unsigned int a = 0xC5F26E0AU;
    unsigned int b = 0x9BA81115U;
    unsigned int c = 0x8C0C4CD4U;
    unsigned int d = 0x665F2162U;
    unsigned int e = 0xE68197D8U;
    sha1Iteration(data, &a, &b, &c, &d, &e, 59);

    TestCase(a == 0x86454801U);
    TestCase(b == 0xC5F26E0AU);
    TestCase(c == 0x66EA0445U);
    TestCase(d == 0x8C0C4CD4U);
    TestCase(e == 0x665F2162U);
}
}

// Test the sha1Block function
{
    SHA1State state;
    initState(&state);

    {
        // Random-looking values for a data block.  If this was a block from the
        // input file, we're assuming the bytes have already been reordered so
        // each 4-byte word is in MSB-first order.
        unsigned char data[64] = {0xB3, 0x7C, 0xB7, 0x92, 0x88, 0x4E, 0x2D,
                                  0x5C, 0xAF, 0x8D, 0x37, 0xFF, 0xEC, 0xF6,
                                  0x7C, 0xB2, 0xCA, 0xDC, 0x7F, 0xD4, 0x03,
                                  0xC4, 0xF1, 0x80, 0xB0, 0x48, 0x60, 0x75,
                                  0x0C, 0xBE, 0xF5, 0x7B, 0xB8, 0x50, 0xFE,
                                  0x46, 0xE8, 0x42, 0xAC, 0xED, 0xB5, 0xAD,
                                  0xA0, 0x3B, 0xA9, 0xEF, 0xE9, 0x2A, 0x57,
                                  0xA5, 0x02, 0x8E, 0x3A, 0xEB, 0x18, 0x69,
                                  0x75, 0x3D, 0xC4, 0xA8, 0xD7, 0x38, 0x7A,
                                  0x71};

        sha1Block(data, &state);

        TestCase(state.h0 == 0x932B5A78U);
        TestCase(state.h1 == 0xBAF3265AU);
        TestCase(state.h2 == 0x0A270F3AU);
        TestCase(state.h3 == 0x33BF5201U);
        TestCase(state.h4 == 0x775154C3U);
    }

    {
        unsigned char data[64] = {0x45, 0x50, 0x33, 0x77, 0x24, 0x0E, 0x6D,
                                  0x9E, 0x86, 0x7F, 0x70, 0x86, 0xCB, 0x2A,
                                  0x3C, 0x23, 0x07, 0x58, 0x4F, 0xDB, 0x3E,
                                  0x62, 0xFA, 0xE0, 0xCA, 0xDA, 0xF4, 0x39,
                                  0xB1, 0x85, 0xB1, 0x65, 0x0D, 0x9E, 0x70,
                                  0x7B, 0x8F, 0xBE, 0xE1, 0xE6, 0xFF, 0xE6,
                                  0xA4, 0xBE, 0x31, 0xC8, 0x7B, 0x53, 0xF8,
                                  0xB1, 0xC3, 0x47, 0x48, 0x41, 0x79, 0x12,
                                  0x24, 0x1F, 0xDE, 0x52, 0x04, 0x1E, 0x0E,
                                  0xEB};

        sha1Block(data, &state);

        TestCase(state.h0 == 0xEEE2C5BEU);
        TestCase(state.h1 == 0xC5352922U);
        TestCase(state.h2 == 0xA1AA1A18U);
        TestCase(state.h3 == 0x91520901U);
        TestCase(state.h4 == 0x4DC3999AU);
    }
}

#ifdef NEVER

#endif

printf("You passed %d / %d unit tests\n", passedTests, totalTests);

if (passedTests != 82)
    return EXIT_FAILURE;
else
    return EXIT_SUCCESS;
}
