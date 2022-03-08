#include "helper.h"
#include <ctype.h>
#include <limits.h>

/**
 * @brief String split function
 *
 * @param pppList The pointer of return list of strings
 * @param pCounter The pointer of length of return list
 * @param pStr The string to be splitted
 * @param pSeparator The separator string
 * @return i32 success (0) or fail (-1)
 */
i32 mystrsplit(string** pppList, i32* pCounter, const string pStr, const string pSeparator) {
    if (pppList == NULL || pCounter == NULL || pStr == NULL || (pSeparator != NULL && strlen(pSeparator) == 0)) {
        if (pppList != NULL) {
            *pppList = NULL;
        }
        return -1;
    }

    if (strlen(pStr) == 0) {
        *pppList = malloc(sizeof(string));
        (*pppList)[0] = malloc(sizeof(char));
        strcpy((*pppList)[0], "");
        *pCounter = 1;
        return 0;
    }

    if (pSeparator == NULL) {
        string str = (string)malloc((strlen(pStr) + 1) * sizeof(char));
        strcpy(str, pStr);
        i32 len = strlen(str), slice = 0;
        *pppList = malloc(len * sizeof(string));
        for (i32 i = 0; i < len; i++) {
            str[i] = isspace(str[i]) ? '\0' : str[i];
            if (str[i]) {
                u32 l = 1;
                while (str[i + l] && !isspace(str[i + l])) l++;
                (*pppList)[slice] = malloc((l + 1) * sizeof(char));
                strncpy((*pppList)[slice], str + i, l);
                (*pppList)[slice][l] = '\0';
                i += l - 1;
                slice++;
            }
        }
        *pCounter = slice;
        free(str);
        *pppList = realloc(*pppList, slice * sizeof(string));
    }
    else {
        *pppList = split(pStr, (u64*)pCounter, (string[]) { pSeparator }, 1);
    }

    return 0;
}
