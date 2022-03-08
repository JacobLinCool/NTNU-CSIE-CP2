#include "helper.h"
#include <ctype.h>
#include <limits.h>

/**
 * @brief strtol(), see "man 3 strtol"
 *
 * @param str
 * @param endptr
 * @param base 2 ~ 36, or 0
 * @return long
 */
long mystrtol(const string str, char** endptr, i32 base) {
    u64 val = 0;
    i8 sign = 1;
    u8 overflow = 0, no_digits = 1;
    char* head = str;

    /**
     * The string may begin with an arbitrary amount of white space
     * (as determined by isspace(3)) followed by a single optional '+'
     * or '-' sign.
     */
    while (isspace(head[0])) {
        head++;
    }
    if (head[0] == '-') {
        sign = -1;
        head++;
    }
    else if (head[0] == '+') {
        head++;
    }
    /**
     * If base is zero or 16, the string may then include a "0x" prefix,
     * and the number will be read in base 16;
     */
    if (base == 0 || base == 16) {
        if (head[0] == '0' && (head[1] == 'x' || head[1] == 'X') && isxdigit(head[2])) {
            base = 16;
            head += 2;
        }
    }
    /**
     * otherwise, a zero base is taken as 10 (decimal) unless the next
     * character is ‘0’, in which case it is taken as 8 (octal).
     */
    if (base == 0) {
        if (head[0] == '0') {
            base = 8;
        }
        else {
            base = 10;
        }
    }

    u64 limit = (sign == 1) ? LONG_MAX : -(u64)LONG_MIN;
    while (base != 1 && base <= 36) {
        i64 tmp = 0;

        if (isdigit(head[0])) {
            tmp = head[0] - '0';
        }
        else if (isalpha(head[0])) {
            tmp = (isupper(head[0]) ? head[0] - 'A' : head[0] - 'a') + 10;
        }
        else {
            break;
        }

        if (tmp >= base) {
            break;
        }

        no_digits = 0;

        if (val > (limit - (u64)tmp) / (u64)base) {
            overflow = 1;
            // break;
        }

        val = val * base + tmp;
        head++;
    }

    /**
     * If endptr is not NULL, strtol() stores the address of the first
     * invalid character in *endptr.  If there were no digits at all,
     * however, strtol() stores the original value of str in *endptr.
     */
    if (endptr != NULL) {
        *endptr = no_digits ? str : head;
    }

    return sign * (overflow ? limit : val);
}

/**
 * @brief strchr(), see "man 3 strchr"
 *
 * @param str
 * @param chr
 * @return char*
 */
char* mystrchr(const string str, i32 chr) {
    i32 len = strlen(str);

    for (i32 i = 0; i <= len; i++) {
        if (str[i] == chr) {
            return str + i;
        }
    }

    return NULL;
}

/**
 * @brief strrchr(), see "man 3 strrchr"
 *
 * @param str
 * @param chr
 * @return char*
 */
char* mystrrchr(const string str, int chr) {
    i32 len = strlen(str);

    for (i32 i = len; i >= 0; i--) {
        if (str[i] == chr) {
            return str + i;
        }
    }

    return NULL;
}

/**
 * @brief strspn(), see "man 3 strspn"
 *
 * @param str
 * @param accept
 * @return size_t
 */
size_t mystrspn(const string str, const string  accept) {
    i32 len = strlen(str);
    u64 spanned = 0;

    for (i32 i = 0; i < len; i++) {
        if (mystrchr(accept, str[i]) == NULL) {
            break;
        }
        spanned++;
    }

    return spanned;
}

/**
 * @brief strcspn(), see "man 3 strcspn"
 *
 * @param str
 * @param reject
 * @return size_t
 */
size_t mystrcspn(const string str, const string reject) {
    i32 len = strlen(str);
    u64 spanned = 0;

    for (i32 i = 0; i < len; i++) {
        if (mystrchr(reject, str[i]) != NULL) {
            break;
        }
        spanned++;
    }

    return spanned;
}

/**
 * @brief strpbrk(), see "man 3 strpbrk"
 *
 * @param str
 * @param accept
 * @return char*
 */
char* mystrpbrk(const string str, const string accept) {
    u64 spanned = mystrcspn(str, accept);
    return spanned == strlen(str) ? NULL : str + spanned;
}

/**
 * @brief strstr(), see "man 3 strstr"
 *
 * @param haystack
 * @param needle
 * @return char*
 */
char* mystrstr(const string haystack, const string needle) {
    i32 haystack_len = strlen(haystack);
    i32 needle_len = strlen(needle);

    /**
     * If needle is an empty string, haystack is returned;
     */
    if (needle_len == 0) {
        return haystack;
    }

    for (i32 i = 0; i <= haystack_len - needle_len; i++) {
        if (strncmp(haystack + i, needle, needle_len) == 0) {
            return haystack + i;
        }
    }

    return NULL;
}

static string __mystrtok_internal_str = NULL;
/**
 * @brief strtok(), see "man 3 strtok"
 *
 * @param str
 * @param delim
 * @return char*
 */
char* mystrtok(string str, const string delim) {
    if (str != NULL) {
        __mystrtok_internal_str = str;
    }

    while (__mystrtok_internal_str[0] && mystrchr(delim, __mystrtok_internal_str[0])) {
        DBG_PRINT("Pre-Matched %c\n", __mystrtok_internal_str[0]);
        __mystrtok_internal_str[0] = '\0';
        __mystrtok_internal_str++;
    }

    if (strlen(__mystrtok_internal_str) == 0) {
        return NULL;
    }

    char* token = __mystrtok_internal_str;
    u8 matched = 0;
    while (!matched && __mystrtok_internal_str[0]) {
        DBG_PRINT("%s\n", __mystrtok_internal_str);
        while (__mystrtok_internal_str[0] && mystrchr(delim, __mystrtok_internal_str[0])) {
            DBG_PRINT("Matched %c\n", __mystrtok_internal_str[0]);
            __mystrtok_internal_str[0] = '\0';
            matched = 1;

            __mystrtok_internal_str++;
        }
        if (matched) {
            break;
        }
        __mystrtok_internal_str++;
    }

    return token[0] ? token : __mystrtok_internal_str;
}
