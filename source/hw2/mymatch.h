#include "helper.h"

#ifndef MYMATCH_H
#define MYMATCH_H

/**
 * @brief Check if the given string matches the given glob pattern.
 *
 * @param str The string to be checked.
 * @param pat The glob pattern.
 */
u8 glob_match(const string restrict str, const string restrict pat) {
    u64 str_len = strlen(str), pat_len = strlen(pat);
    u64 str_idx = 0, pat_idx = 0;
    u64 str_idx_retry = 0, pat_idx_retry = 0;

    while (str_idx < str_len || pat_idx < pat_len) {
        DBG_PRINT("---\nstr: %s\npat: %s\n", str + str_idx, pat + pat_idx);
        if (pat_idx < pat_len) {
            if (pat[pat_idx] == '*') {
                pat_idx_retry = pat_idx;
                str_idx_retry = str_idx + 1;
                pat_idx++;
                continue;
            }
            else if (pat[pat_idx] == '?' && str_idx < str_len) {
                str_idx++, pat_idx++;
                continue;
            }
            else if (str[str_idx] == pat[pat_idx]) {
                str_idx++, pat_idx++;
                continue;
            }
        }

        if (str_idx_retry > 0 && str_idx_retry <= str_len) {
            str_idx = str_idx_retry;
            pat_idx = pat_idx_retry;
            continue;
        }

        return 0;
    }

    return 1;
}

/**
 * @brief Match strings against a glob pattern.
 *
 * @param pppList The list of strings that matched the glob pattern.
 * @param pStr The string to be checked.
 * @param pPattern The glob pattern.
 * @return The number of matches.
 */
i32 mymatch(string* pList[], const string pStr, const string pPattern) {
    if (pList == NULL || pStr == NULL || pPattern == NULL) {
        return -1;
    }
    if (strlen(pPattern) == 0) {
        return -1;
    }

    string str = malloc(strlen(pStr) + 1);
    strcpy(str, pStr);

    i32 size = 0;
    *pList = (string*)malloc(strlen(str) + 1);
    string token = strtok(str, " ");
    while (token != NULL) {
        if (glob_match(token, pPattern)) {
            (*pList)[size] = malloc(strlen(token) + 1);
            strcpy((*pList)[size], token);
            size++;
        }
        token = strtok(NULL, " ");
    }

    free(str);

    return size;
}

#endif // MYMATCH_H
