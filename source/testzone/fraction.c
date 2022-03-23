// #define DEBUG
#include "fraction.h"
#include "helper.h"

static inline i64 gcd(i64 a, i64 b) {
    if (b) while ((a %= b) && (b %= a));
    return a + b;
}

void mixed_add(sMixedNumber* mixed, const sMixedNumber r1, const sMixedNumber r2) {
    mixed->numerator = r1.numerator * r2.denominator + r2.numerator * r1.denominator;
    mixed->denominator = r1.denominator * r2.denominator;
    normalize_mixed(mixed);
}

void mixed_sub(sMixedNumber* mixed, const sMixedNumber r1, const  sMixedNumber r2) {
    mixed->numerator = r1.numerator * r2.denominator - r2.numerator * r1.denominator;
    mixed->denominator = r1.denominator * r2.denominator;
    normalize_mixed(mixed);
}

void mixed_mul(sMixedNumber* mixed, const sMixedNumber r1, const sMixedNumber r2) {
    mixed->numerator = r1.numerator * r2.numerator;
    mixed->denominator = r1.denominator * r2.denominator;
    normalize_mixed(mixed);
}

void mixed_div(sMixedNumber* mixed, const sMixedNumber r1, const  sMixedNumber r2) {
    mixed->numerator = r1.numerator * r2.denominator;
    mixed->denominator = r1.denominator * r2.numerator;
    normalize_mixed(mixed);
}

sMixedNumber* parse_mixed(const char* restrict str, i64* end) {
    if (!str || str[0] == '\0') return NULL;

    DBG_PRINT("[Parser] Parsing \"%s\"\n", str);

    i64 inte = 0, nume = 0, deno = 1;

    string matchers[] = {
        "(%" PRId64 "\\frac{%" PRId64 "}{%" PRId64 "})",
        "%" PRId64 "\\frac{%" PRId64 "}{%" PRId64 "}",
        "(\\frac{%" PRId64 "}{%" PRId64 "})",
        "\\frac{%" PRId64 "}{%" PRId64 "}",
        "(%" PRId64 ")",
        "%" PRId64 "",
    };

    i64* matches[][4] = {
        { &inte, &nume, &deno, end },
        { &inte, &nume, &deno, end },
        { &nume, &deno, end, NULL },
        { &nume, &deno, end, NULL },
        { &inte, end, NULL, NULL },
        { &inte, end, NULL, NULL },
    };

    i64 counts[] = { 3, 3, 2, 2, 1, 1 };

    for (i64 i = 0; i < 6; i++) {
        string matcher = (string)calloc(strlen(matchers[i]) + 2 + 1, sizeof(char));
        strcpy(matcher, matchers[i]);
        strcat(matcher, "%n");
        if (sscanf(str, matcher, matches[i][0], matches[i][1], matches[i][2], matches[i][3]) == counts[i]) {
            DBG_PRINT("[Parser] Matched Pattern = \"%s\"\n", matcher);
            DBG_PRINT("[Parser] End Point = %" PRId64 "\n", *end);
            free(matcher);
            break;
        }
        free(matcher);
    }

    DBG_PRINT("[Parser] inte = %" PRId64 ", nume = %" PRId64 ", deno = %" PRId64 "\n", inte, nume, deno);

    sMixedNumber* number = (sMixedNumber*)malloc(sizeof(sMixedNumber));
    number->numerator = inte * deno + (inte < 0 ? -nume : nume);
    number->denominator = deno;

    normalize_mixed(number);
    return number;
}

string mixed_to_string(const sMixedNumber* mixed) {
    sMixedNumber* number = (sMixedNumber*)malloc(sizeof(sMixedNumber));
    number->numerator = mixed->numerator;
    number->denominator = mixed->denominator;

    string neg_prefix = number->numerator < 0 ? "-" : "";
    if (number->numerator < 0) {
        number->numerator = -number->numerator;
    }

    string str = (string)calloc(1024, sizeof(char));

    if (number->numerator == 0) {
        sprintf(str, "0");
    }
    else if (number->denominator == 1) {
        sprintf(str, "%" PRId64, number->numerator);
    }
    else if (number->numerator < number->denominator) {
        sprintf(
            str,
            "\\frac{%s%" PRId64 "}{%" PRId64 "}",
            neg_prefix,
            number->numerator,
            number->denominator
        );
    }
    else {
        sprintf(
            str,
            "%s%" PRId64 "\\frac{%" PRId64 "}{%" PRId64 "}",
            neg_prefix,
            number->numerator / number->denominator,
            number->numerator % number->denominator,
            number->denominator
        );
    }

    free(number);
    str = (string)realloc(str, strlen(str) + 1);

    return str;
}

void normalize_mixed(sMixedNumber* mixed) {
    i64 g = gcd(mixed->numerator, mixed->denominator);
    mixed->numerator /= g, mixed->denominator /= g;
    if (mixed->denominator < 0) {
        mixed->numerator *= -1, mixed->denominator *= -1;
    }
}
