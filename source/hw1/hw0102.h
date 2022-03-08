/**
 * @brief An interactive command line program that replaces all occurrences of a substring in an input with a replacement string.
 * Provide 3 strings: the full text, the substring to be replaced, and the replacement string. We will handle the rest.
 *
 * Support highligting of the keyword and the replacement string.
 */
#include <ctype.h>
#include "helper.h"

string highlight(const string restrict full_text, const u64* restrict pos, const u64 size, const u64 len, const string restrict color);

u64* find(const string restrict full_text, const string restrict keyword, u64* restrict count);

string str_to_lower(const string restrict str);
