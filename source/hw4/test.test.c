#include "test.h"
#include "lib/cimple-lib/all.h"

void print_bits(uint8_t* buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < 8; j++) {
            printf("%d", (buffer[i] >> (7 - j)) & 1);
        }
        printf(" ");
        if (i % 4 == 3) {
            printf("\n");
        }
    }
    printf("\n");
}

// ip
i32 main() {
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

// // tcp
// i32 main() {
//     TEST* test = test_init();
//     uint8_t data[] = {
//         0b10000000,
//         0b11000000,
//         0b11100000,
//         0b11110000,
//         0b11111000,
//         0b11111100,
//         0b11111110,
//         0b11111111,
//         0b01111111,
//         0b00111111,
//         0b00011111,
//         0b00001111,
//         0b00000111,
//         0b00000011,
//         0b00000001,
//         0b00000000,
//         0b10000000,
//         0b11000000,
//         0b11100000,
//         0b11110000,
//         0b11111000,
//         0b11111100,
//         0b11111110,
//         0b11111111,
//         0b01111111,
//         0b00111111,
//         0b00011111,
//         0b00001111,
//     };
//     print_bits(data, sizeof(data));
//     printf("\n");

//     assert(test != NULL);
//     assert(test_decode(NULL, NULL) == -1);
//     assert(test_decode(&data, NULL) == -1);
//     assert(test_decode(NULL, test) == -1);
//     assert(test_decode(&data, test) == 0);
//     print_bits(&test->sourceport, sizeof(test->sourceport));
//     print_bits(&test->destinationport, sizeof(test->destinationport));
//     print_bits(&test->sequencenumber, sizeof(test->sequencenumber));
//     print_bits(&test->acknowledgmentnumber, sizeof(test->acknowledgmentnumber));
//     print_bits(&test->offset, sizeof(test->offset));
//     print_bits(&test->reserved, sizeof(test->reserved));
//     print_bits(&test->u, sizeof(test->u));
//     print_bits(&test->a, sizeof(test->a));
//     print_bits(&test->p, sizeof(test->p));
//     print_bits(&test->r, sizeof(test->r));
//     print_bits(&test->s, sizeof(test->s));
//     print_bits(&test->f, sizeof(test->f));
//     print_bits(&test->window, sizeof(test->window));
//     print_bits(&test->checksum, sizeof(test->checksum));
//     print_bits(&test->urgentpointer, sizeof(test->urgentpointer));
//     print_bits(&test->options, sizeof(test->options));
//     print_bits(&test->padding, sizeof(test->padding));
//     print_bits(&test->data, sizeof(test->data));

//     uint8_t* buffer = calloc(28, 1);
//     assert(buffer != NULL);
//     assert(test_encode(NULL, NULL) == -1);
//     assert(test_encode(buffer, NULL) == -1);
//     assert(test_encode(NULL, test) == -1);
//     assert(test_encode(buffer, test) == 0);
//     print_bits(buffer, 28);

//     test_free(test);
//     return 0;
// };
