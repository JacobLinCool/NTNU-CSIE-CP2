#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int main() {
    int64_t number = 0;

    scanf("%" PRId64, &number);

    int64_t bit = 1;
    bit = bit << 31;

    for (int i = 0; i < 32; i++) {
        if (bit & number)
            printf("1");
        else
            printf("0");
        bit = bit >> 1;
    }
    return 0;
}