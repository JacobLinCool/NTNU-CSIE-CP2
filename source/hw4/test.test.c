#include "test.h"
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IP 0
#define TCP 1
#define IP_S 2
#define TCP_S 3
#ifndef TYPE
#define TYPE TCP_S
#endif

void print_bits(uint8_t* buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < 8; j++) {
            printf("%d", (buffer[i] >> j) & 1);
        }
        printf(" ");
        if (i % 4 == 3) {
            printf("\n");
        }
    }
    printf("\n");
}

bool buffer_get_bit(const void* buffer, size_t bit) {
    if (buffer == NULL) {
        return false;
    }

    const uint8_t* buffer_ptr = buffer;
    size_t         byte = bit / 8;
    size_t         offset = bit % 8;
    return (buffer_ptr[byte] >> offset) & 1;
}

char* buffer_stringify(const void* buffer, size_t size, size_t col_size, size_t row_size) {
    if (buffer == NULL) {
        return NULL;
    }

    size_t cols = (size + col_size - 1) / col_size;
    size_t rows = (cols + row_size - 1) / row_size;
    char* str = malloc(size + cols + rows + 1);

    size_t idx = 0;
    for (size_t i = 0; i < size; i++) {
        str[idx++] = buffer_get_bit(buffer, i) ? '1' : '0';
        if (i % col_size == col_size - 1) {
            str[idx++] = ' ';
        }
        if (i % (col_size * row_size) == (col_size * row_size) - 1 && i != size - 1) {
            str[idx++] = '\n';
        }
    }
    str[idx] = '\0';

    return str;
}

#if TYPE == IP
int main() {
    TEST* test = test_init();
    printf("Source:\n");
    uint8_t data[] = {
        0b10000000,
        0b11000000,
        0b11100000,
        0b11110000,
        0b11111000,
        0b11111100,
        0b11111110,
        0b11111111,
        0b01111111,
        0b00111111,
        0b00011111,
        0b00001111,
        0b00000111,
        0b00000011,
        0b00000001,
        0b00000000,
        0b10000000,
        0b11000000,
        0b11100000,
        0b11110000,
        0b11111000,
        0b11111100,
        0b11111110,
        0b11111111,
    };
    print_bits(data, sizeof(data));
    printf("\n");

    printf("Decoded:\n");
    assert(test != NULL);
    assert(test_decode(NULL, NULL) == -1);
    assert(test_decode(&data, NULL) == -1);
    assert(test_decode(NULL, test) == -1);
    assert(test_decode(&data, test) == 0);
    printf("version: ");
    print_bits(&test->version, sizeof(test->version));
    printf("ihl: ");
    print_bits(&test->ihl, sizeof(test->ihl));
    printf("typeofservice: ");
    print_bits(&test->typeofservice, sizeof(test->typeofservice));
    printf("totallength: ");
    print_bits(&test->totallength, sizeof(test->totallength));
    printf("identification: ");
    print_bits(&test->identification, sizeof(test->identification));
    printf("flags: ");
    print_bits(&test->flags, sizeof(test->flags));
    printf("fragmentoffset: ");
    print_bits(&test->fragmentoffset, sizeof(test->fragmentoffset));
    printf("timetolive: ");
    print_bits(&test->timetolive, sizeof(test->timetolive));
    printf("protocol: ");
    print_bits(&test->protocol, sizeof(test->protocol));
    printf("headerchecksum: ");
    print_bits(&test->headerchecksum, sizeof(test->headerchecksum));
    printf("sourceaddress: ");
    print_bits(&test->sourceaddress, sizeof(test->sourceaddress));
    printf("destinationaddress: ");
    print_bits(&test->destinationaddress, sizeof(test->destinationaddress));
    printf("options: ");
    print_bits(&test->options, sizeof(test->options));
    printf("padding: ");
    print_bits(&test->padding, sizeof(test->padding));
    printf("\n");

    printf("Encoded:\n");
    uint8_t* buffer = calloc(sizeof(data), 1);
    assert(buffer != NULL);
    assert(test_encode(NULL, NULL) == -1);
    assert(test_encode(buffer, NULL) == -1);
    assert(test_encode(NULL, test) == -1);
    assert(test_encode(buffer, test) == 0);
    print_bits(buffer, sizeof(data));

    test_free(test);
    return 0;
};
#endif

#if TYPE == TCP
int main() {
    TEST* test = test_init();
    uint8_t data[] = {
        0b10000000,
        0b11000000,
        0b11100000,
        0b11110000,
        0b11111000,
        0b11111100,
        0b11111110,
        0b11111111,
        0b01111111,
        0b00111111,
        0b00011111,
        0b00001111,
        0b00000111,
        0b00000011,
        0b00000001,
        0b00000000,
        0b10000000,
        0b11000000,
        0b11100000,
        0b11110000,
        0b11111000,
        0b11111100,
        0b11111110,
        0b11111111,
        0b01111111,
        0b00111111,
        0b00011111,
        0b00001111,
    };
    print_bits(data, sizeof(data));
    printf("\n");

    assert(test != NULL);
    assert(test_decode(NULL, NULL) == -1);
    assert(test_decode(&data, NULL) == -1);
    assert(test_decode(NULL, test) == -1);
    assert(test_decode(&data, test) == 0);
    print_bits(&test->sourceport, sizeof(test->sourceport));
    print_bits(&test->destinationport, sizeof(test->destinationport));
    print_bits(&test->sequencenumber, sizeof(test->sequencenumber));
    print_bits(&test->acknowledgmentnumber, sizeof(test->acknowledgmentnumber));
    print_bits(&test->offset, sizeof(test->offset));
    print_bits(&test->reserved, sizeof(test->reserved));
    print_bits(&test->u, sizeof(test->u));
    print_bits(&test->a, sizeof(test->a));
    print_bits(&test->p, sizeof(test->p));
    print_bits(&test->r, sizeof(test->r));
    print_bits(&test->s, sizeof(test->s));
    print_bits(&test->f, sizeof(test->f));
    print_bits(&test->window, sizeof(test->window));
    print_bits(&test->checksum, sizeof(test->checksum));
    print_bits(&test->urgentpointer, sizeof(test->urgentpointer));
    print_bits(&test->options, sizeof(test->options));
    print_bits(&test->padding, sizeof(test->padding));
    print_bits(&test->data, sizeof(test->data));

    uint8_t* buffer = calloc(28, 1);
    assert(buffer != NULL);
    assert(test_encode(NULL, NULL) == -1);
    assert(test_encode(buffer, NULL) == -1);
    assert(test_encode(NULL, test) == -1);
    assert(test_encode(buffer, test) == 0);
    print_bits(buffer, 28);

    test_free(test);
    return 0;
};
#endif

#if TYPE == IP_S
int main() {
    TEST* test = test_init();
    test->version = 4;
    test->ihl = 5;
    test->typeofservice = 0U;
    test->totallength = 40U;
    test->identification = 0U;
    test->flags = 4U;
    *(uint16_t*)test->fragmentoffset = 0U;
    test->timetolive = 64U;
    test->protocol = 6U;
    test->headerchecksum = 0x222cU;
    test->sourceaddress = 0xc0a83272U;
    test->destinationaddress = 0xa29f82eaU;
    *(uint32_t*)test->options = 123456U;
    test->padding = 111U;

    uint8_t* buffer = calloc(24, 1);
    assert(buffer != NULL);
    assert(test_encode(buffer, test) == 0);
    // print_bits(buffer, 24);
    printf("\n");
    FILE* fp = fopen("test.bin", "wb");
    fwrite(buffer, 1, 24, fp);
    fclose(fp);
    system("xxd -b test.bin");

    TEST* test2 = test_init();
    assert(test_decode(buffer, test2) == 0);

#define TEST_EQUAL(field) ({ \
    printf("%s : (%" PRIu32 ") ", #field, test->field); \
    printf("%s\n", buffer_stringify(&test->field, sizeof(test->field) * 8, 8, 4)); \
    printf("%s : (%" PRIu32 ") ", #field, test2->field); \
    printf("%s\n", buffer_stringify(&test2->field, sizeof(test2->field) * 8, 8, 4)); \
    assert(memcmp(&test->field, &test2->field, sizeof(test->field)) == 0); \
})

    TEST_EQUAL(version);
    TEST_EQUAL(ihl);
    TEST_EQUAL(typeofservice);
    TEST_EQUAL(totallength);
    TEST_EQUAL(identification);
    TEST_EQUAL(flags);
    TEST_EQUAL(fragmentoffset);
    TEST_EQUAL(timetolive);
    TEST_EQUAL(protocol);
    TEST_EQUAL(headerchecksum);
    TEST_EQUAL(sourceaddress);
    TEST_EQUAL(destinationaddress);
    TEST_EQUAL(options);
    TEST_EQUAL(padding);

    printf("%s\n", buffer_stringify(buffer, 24 * 8, 8, 4));

    return EXIT_SUCCESS;
}
#endif


#if TYPE == TCP_S
int main() {
    TEST* test = test_init();
    test->sourceport = 443U;
    test->destinationport = 50158U;
    test->sequencenumber = 3814881307U;
    test->acknowledgmentnumber = 2981612091U;
    test->offset = 0b1000U;
    test->reserved = 0U;
    test->u = 0U;
    test->a = 1U;
    test->p = 0U;
    test->r = 0U;
    test->s = 0U;
    test->f = 0U;
    test->window = 352U;
    test->checksum = 0x8186U;
    test->urgentpointer = 0U;
    *(uint32_t*)test->options = 0x080101;
    test->padding = 0U;
    test->data = 0x01010101U;

    uint8_t* buffer = calloc(28, 1);
    assert(buffer != NULL);
    assert(test_encode(buffer, test) == 0);
    // print_bits(buffer, 24);
    printf("\n");
    FILE* fp = fopen("test.bin", "wb");
    fwrite(buffer, 1, 28, fp);
    fclose(fp);
    system("xxd -b test.bin");

    TEST* test2 = test_init();
    assert(test_decode(buffer, test2) == 0);

#define TEST_EQUAL(field) ({ \
    printf("%s : (%" PRIu32 ") ", #field, test2->field); \
    printf("%s\n", buffer_stringify(&test2->field, sizeof(test2->field) * 8, 8, 4)); \
    assert(memcmp(&test->field, &test2->field, sizeof(test->field)) == 0); \
})

    TEST_EQUAL(sourceport);
    TEST_EQUAL(destinationport);
    TEST_EQUAL(sequencenumber);
    TEST_EQUAL(acknowledgmentnumber);
    TEST_EQUAL(offset);
    TEST_EQUAL(reserved);
    TEST_EQUAL(u);
    TEST_EQUAL(a);
    TEST_EQUAL(p);
    TEST_EQUAL(r);
    TEST_EQUAL(s);
    TEST_EQUAL(f);
    TEST_EQUAL(window);
    TEST_EQUAL(checksum);
    TEST_EQUAL(urgentpointer);
    TEST_EQUAL(options);
    TEST_EQUAL(padding);
    TEST_EQUAL(data);

    printf("%s\n", buffer_stringify(buffer, 28 * 8, 8, 4));

    return EXIT_SUCCESS;
}
#endif
