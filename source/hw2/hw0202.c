#include "hw0202.h"
#include "helper.h"

string u64_to_binary(u64 n) {
    string res = calloc(64, sizeof(char));
    for (i32 i = 63; i >= 0; i--) {
        res[i] = (n & 1) + '0';
        n >>= 1;
    }
    return res;
}

string fraction_to_polynomial(const string fraction) {
    string str = calloc(10000, sizeof(char));
    strcat(str, "1");
    for (i32 i = 0; i < strlen(fraction); i++) {
        if (fraction[i] == '1') {
            string tmp = calloc(100, sizeof(char));
            sprintf(tmp, " + 2^-%" PRId32, i + 1);
            strcat(str, tmp);
            free(tmp);
        }
    }
    return realloc(str, strlen(str) + 1);
}

i32 main() {
    f64 raw = 0.0;
    printf("Please enter a floating-point number (double precision): ");
    scanf("%lf", &raw);

    u64 n = *(u64*)&raw;
    string binary = u64_to_binary(n);
    string sign = malloc(2 * sizeof(char));
    strncpy(sign, binary, 1);
    string exponent = malloc(12 * sizeof(char));
    strncpy(exponent, binary + 1, 11);
    string fraction = malloc(53 * sizeof(char));
    strncpy(fraction, binary + 12, 52);

    printf("Sign: %s\n", sign);
    printf("Exponent: %s\n", exponent);
    printf("Fraction: %s\n", fraction);
    printf(
        "%lg = (-1)^%s * (%s) * 2^(%" PRId64 "-1023)\n",
        raw,
        sign,
        fraction_to_polynomial(fraction),
        (n >> 52) & (((u64)1 << 11) - 1)
    );

    return 0;
}
