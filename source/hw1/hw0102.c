// #define DEBUG
#define FULL_TEXT_LEN 5000
#define SUB_STR_LEN 100
#include "hw0102.h"

void input(string full_text, string keyword, string replacement) {
    printf("Please enter the string: \n");
    fgets(full_text, FULL_TEXT_LEN, stdin);
    full_text[strlen(full_text) - 1] = '\0';
    DBG_PRINT("full_text = [%s]\n", full_text);

    printf("Please enter the keyword: \n");
    fgets(keyword, SUB_STR_LEN, stdin);
    keyword[strlen(keyword) - 1] = '\0';
    DBG_PRINT("keyword = [%s]\n", keyword);

    printf("Please enter the new word: \n");
    fgets(replacement, SUB_STR_LEN, stdin);
    replacement[strlen(replacement) - 1] = '\0';
    DBG_PRINT("replacement = [%s]\n", replacement);
}

i32 main() {
    string full_text = (string)calloc(FULL_TEXT_LEN, sizeof(char));
    string keyword = (string)calloc(SUB_STR_LEN, sizeof(char));
    string replacement = (string)calloc(SUB_STR_LEN, sizeof(char));

    input(full_text, keyword, replacement);

    u64 found = 0, key_len = strlen(keyword);
    u64* founds = find(str_to_lower(full_text), str_to_lower(keyword), &found);
    DBG_PRINT("found = %" PRIu64 "\n", found);
    DBG_PRINT("\n%s\n", format_array(u64, founds, 0, found - 1));

    u64 last = 0;
    string* slices = (string*)calloc(found + 1, sizeof(string));
    for (u64 i = 0; i < found; i++) {
        u64 len = founds[i] - last;
        slices[i] = (string)calloc(len + 1, sizeof(char));
        strncpy(slices[i], full_text + last, len);
        last = founds[i] + key_len;
    }
    slices[found] = (string)calloc(strlen(full_text) - last + 1, sizeof(char));
    strcpy(slices[found], full_text + last);
    DBG_PRINT("\n%s\n", format_array(str, slices, 0, found));

    printf("Original:\n%s\n", highlight(full_text, founds, found, key_len, CLR_RED));
    printf("New:\n%s\n", join(slices, found + 1, tint(replacement, CLR_GREEN)));

    return 0;
}

string highlight(const string restrict full_text, const u64* restrict pos, const u64 size, const u64 len, const string restrict color) {
    if (size <= 0) {
        return full_text;
    }
    string highlighted = (string)calloc(strlen(full_text) + (strlen(color) + strlen(CLR_NONE)) * size + 1, sizeof(char));
    strncpy(highlighted, full_text, pos[0]);
    for (u64 i = 0; i < size; i++) {
        strcat(highlighted, color);
        strncat(highlighted, full_text + pos[i], len);
        strcat(highlighted, CLR_NONE);
        if (i < size - 1) {
            strncat(highlighted, full_text + pos[i] + len, pos[i + 1] - pos[i] - len);
        }
        else {
            strcat(highlighted, full_text + pos[i] + len);
        }
    }

    return highlighted;
}

u64* find(const string restrict full_text, const string restrict keyword, u64* restrict count) {
    u64* positions = (u64*)calloc(FULL_TEXT_LEN, sizeof(u64));
    u64 index = 0;
    for (u64 i = 0; i < strlen(full_text); i++) {
        if (full_text[i] == keyword[0]) {
            bool match = true;
            for (u64 j = 0; j < strlen(keyword); j++) {
                if (full_text[i + j] != keyword[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                positions[index++] = i;
            }
        }
    }
    positions[index] = 0;
    positions = (u64*)realloc(positions, (index + 1) * sizeof(u64));
    *count = index;
    return positions;
}

string str_to_lower(const string restrict str) {
    string lower = (string)calloc(strlen(str) + 1, sizeof(char));
    for (u64 i = 0; i < strlen(str); i++) {
        lower[i] = tolower(str[i]);
    }
    return lower;
}
