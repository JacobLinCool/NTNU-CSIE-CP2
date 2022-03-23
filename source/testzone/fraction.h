#include "helper.h"

#ifndef FRACTION_H
#define FRACTION_H

typedef struct _sMixedNumber {
    i64 numerator;
    i64 denominator;
} sMixedNumber;

void mixed_add(sMixedNumber* mixed, const sMixedNumber r1, const  sMixedNumber r2);
void mixed_sub(sMixedNumber* mixed, const sMixedNumber r1, const  sMixedNumber r2);
void mixed_mul(sMixedNumber* mixed, const sMixedNumber r1, const sMixedNumber r2);
void mixed_div(sMixedNumber* mixed, const sMixedNumber r1, const  sMixedNumber r2);

sMixedNumber* parse_mixed(const char* restrict str, i64* end);
string mixed_to_string(const sMixedNumber* mixed);
void normalize_mixed(sMixedNumber* mixed);

#endif // FRACTION_H
