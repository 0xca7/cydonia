#include <CppUTest/TestHarness.h>
#include "CppUTest/TestHarness_c.h"
#include <CppUTestExt/MockSupport.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern "C"
{
    #include "../../cydonia/cydonia.c"
}

TEST_GROUP(Cydonia)
{
    void setup() {
    }


    void teardown() {
    }

};

TEST(Cydonia, cydonia_read_write_file) {
    
    const char *buffer = "test";

    void *p = NULL;
    size_t size = 0;
    p = cydonia_read_file("/tmp/thisdoesnotexist", &size);
    CHECK(p == NULL);

    CHECK(!cydonia_write_file("/tmp/cydonia_testfile", (const void*)buffer, 4));

    p = cydonia_read_file("/tmp/cydonia_testfile", &size);
    CHECK(p != NULL);
    CHECK(!memcmp(p, "test", 4));

    free(p);
}

TEST(Cydonia, cydonia_hexdump) {
    
    usleep(1000);

    const uint8_t bytes0[] = {
        0xcf,0xee,0x46,0x70,0x81,0x4b,0x1c,0x44,
        0xb7,0x2f,0x3c,0x02,0x70,0xc5,0x86,0xae,
        0xdc,0xff,0xd1,0x67,0xef,0x49,0x68,0xe7,
        0xe0,0x1c,0x5e,0xfc,0x01,0x6d,0x67,0x48,
        0x7e,0x6a,0x88,0x76,0x4a,0x9e,0x51,0x05,
        0xc1,0x7f,0x49,0x95,0xad,0x94,0x89,0x87,
        0x00,0x35,0xaf,0xd9,0x57,0x5f,0x25,0x38,
        0xc1,0xcf,0x06,0xbb,0x78,0x46,0xd2,0xbd,
        0x40,0x69
    };

    const uint8_t bytes1[] = {
        0xcf,0xee,0x46
    };

    const uint8_t bytes2[] = {
        0xcf,0xee,0x46,0x70,0x81,0x4b,0x1c,0x44,
        0xb7,0x2f,0x3c,0x02,0x70,0xc5,0x86,0xae
    };

    cydonia_hexdump(bytes0, sizeof(bytes0));
    cydonia_hexdump(bytes1, sizeof(bytes1));
    cydonia_hexdump(bytes2, sizeof(bytes2));

    cydonia_hexdump(NULL, sizeof(bytes2));
    cydonia_hexdump_to_file("/tmp/testfile", bytes0, sizeof(bytes0));

}

TEST(Cydonia, cydonia_printbinary) {

    uint8_t  num0  = 0x55;
    uint16_t num1 = 0x55aa;
    uint32_t num2 = 0xdeadbeef;
    uint64_t num3 = 0xcafebabebaadf00d;
    cydonia_print_binary(num0, BITS8);
    cydonia_print_binary(num1, BITS16);
    cydonia_print_binary(num2, BITS32);
    cydonia_print_binary(num3, BITS64);

    cydonia_print_binary_verbose(num0, BITS8);
    cydonia_print_binary_verbose(num1, BITS16);
    cydonia_print_binary_verbose(num2, BITS32);
    cydonia_print_binary_verbose(num3, BITS64);

}