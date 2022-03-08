#include "hw0103.h"

void test(const string str, i32 base) {
    // printf(CLR_CYAN "Test: str = \"%s\", base = %" PRId32 CLR_NONE "\n", str, base);
    char** endptr = calloc(1, sizeof(char*));
    char** my_endptr = calloc(1, sizeof(char*));
    long ans = strtol(str, endptr, base);
    long my_ans = mystrtol(str, my_endptr, base);
    if (ans != my_ans || *endptr != *my_endptr) {
        printf(CLR_YELLOW "Test Failed: str = \"%s\", base = %" PRId32 CLR_NONE "\n", str, base);
        printf(CLR_GREEN "[EXPECTED]" CLR_NONE " value = %20ld, end = %3ld [%c]\n", ans, *endptr - str, *endptr[0]);
        printf(CLR_RED "[RECEIVED]" CLR_NONE " value = %20ld, end = %3ld [%c]\n", my_ans, *my_endptr - str, *my_endptr[0]);
        exit(EXIT_FAILURE);
    }
}

i32 main() {
    string strs[] = {
        "101101010100101", // 2
        "123874374824187", // 8
        "123456789abcdef", // 16
        "123456789ABCDEF", // 16 Big
        "12345z78yabcdxfg", // 36
        "003456789abcdef", // starts with 0
        "00345!789!bcdef", // with !
        "", // empty
        "00",
        // NULL, // NULL
        "0x123", // hex
        "-123", // negative
        "-0x123", // negative hex
        "zZzZzZzZzZzZzZzZzZzZzZzZzZzZzZzZ", // overflow
        "-9223372036854775808", // LONG_MIN
        "9223372036854775807", // LONG_MAX
        "9223372036854775808", // overflow
        "-9223372036854775809", // overflow
        "     ", // white spaces
        "     -123     ", // white spaces arounded
        "     -12   34   ", // white spaces between
        "   -  1234  ",
        "0xga", // invalid hex
        "092", // invalid octal
        "0x", // hex prefix only
    };

    for (i32 i = 0; i <= 37; i++) {
        for (i32 j = 0; j < 24; j++) {
            test(strs[j], i);
        }

        // printf(CLR_MAGENTA "Passed base = %" PRId32 CLR_NONE "\n", i);
    }

    printf(CLR_GREEN "All Tests Passed!\n" CLR_NONE);

    return 0;
}
