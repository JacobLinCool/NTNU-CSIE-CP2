#ifndef __UTILS_STRING_H
#define __UTILS_STRING_H

#include "base.h"

typedef struct {
    /** Remove the left and right spaces of a string. */
    string (*trim)(const restrict string str);
    /** Transform a string to lowercase. */
    string (*lower)(const restrict string str);
    /** Turn a string into uppercase. */
    string (*upper)(const restrict string str);
} StringUtils;

string __string_trim(const restrict string str) {
    THROW_IF(str == NULL, "Cannot trim a null string.");

    string trimmed = calloc(strlen(str) + 1, sizeof(char));

    i32 left = 0, right = strlen(str) - 1;
    while (str[left] == ' ') left++;
    while (str[right] == ' ') right--;

    for (i32 i = left; i <= right; i++) trimmed[i - left] = str[i];

    return realloc(trimmed, (right - left + 2) * sizeof(char));
}

string __string_lower(const restrict string str) {
    THROW_IF(str == NULL, "Cannot lower a null string.");

    string lower = calloc(strlen(str) + 1, sizeof(char));

    for (i32 i = 0; i < strlen(str); i++) lower[i] = tolower(str[i]);

    return lower;
}

string __string_upper(const restrict string str) {
    THROW_IF(str == NULL, "Cannot upper a null string.");

    string upper = calloc(strlen(str) + 1, sizeof(char));

    for (i32 i = 0; i < strlen(str); i++) upper[i] = toupper(str[i]);

    return upper;
}

StringUtils String = {.trim = __string_trim, .lower = __string_lower, .upper = __string_upper};

#endif  // __UTILS_STRING_H
