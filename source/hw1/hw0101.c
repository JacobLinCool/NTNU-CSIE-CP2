// #define DEBUG
#include "hw0101.h"

i32 main() {
    u32 output_length = 0;
    string output = (string)calloc((u64)1e7 + 1, sizeof(char));

    printf("Please enter the hex string: ");

    while (1) {
        char hex[3] = { 0 };
        scanf("%2s", hex);
        DBG_PRINT("hex = %s\n", hex);
        if (hex[0] == '0' && hex[1] == '0') {
            break;
        }

        u8 valid = strspn(hex, "0123456789ABCDEFabcdef");
        DBG_PRINT("valid = %" PRIu8 "\n", valid);
        THROW_IF(valid != 2, "Invalid Input. Received: %s\n", hex);

        u8 code = strtol(hex, NULL, 16);
        u8 decoded = decode(code);
        DBG_PRINT("code = %" PRIu8 "\n", code);
        DBG_PRINT("decoded = %" PRIu8 "\n", decoded);
        THROW_IF(decoded < 32 || decoded > 126, "Not Printable. Code: %" PRIu8 "\n", decoded);

        output[output_length++] = decoded;
    }

    printf("%s\n", output);

    return 0;
}

u8 decode(u8 code) {
    u8 rev = 0;
    for (u8 i = 0; i < 8; i++) {
        rev |= ((code >> i) & 1) << (7 - i);
    }
    return rev;
}
