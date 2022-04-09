// #define DEBUG
#include "utils.h"

char* mystrtok_r(char* str, const char* delim, char** saveptr) {
    char* token;

    if (str == NULL) {
        str = *saveptr;
    }

    str += strspn(str, delim);

    if (*str == '\0') {
        return NULL;
    }

    token = str;
    str = strpbrk(token, delim);

    if (str == NULL) {
        *saveptr = token + strlen(token);
    }
    else {
        *str = '\0';
        *saveptr = str + 1;
    }

    return token;
}
