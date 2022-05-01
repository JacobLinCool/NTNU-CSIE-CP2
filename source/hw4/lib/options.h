#ifndef __UTILS_OPTIONS_H
#define __UTILS_OPTIONS_H

#include "cimple-lib/all.h"

typedef struct JOption {
    char short_name;
    string long_name;
    bool has_value;
    bool is_optional;
    u32 idx;
} JOption;

/**
 * @brief Parse command line arguments
 *
 * @param argc Number of arguments
 * @param argv Array of arguments
 * @param options Array of options, in format of <short>[|long][:][?], : implies has value, ? implies optional
 * @param option_count Number of options
 * @param parsed Array of parsed options
 * @return 0 on success, 1 on failure
 */
u8 parse_options(i32 argc, string argv[], string options[], i32 option_count, string parsed[]) {
    JOption joptions[option_count];
    for (i32 i = 0; i < option_count; i++) {
        string option = options[i];
        joptions[i].short_name = option[0];
        joptions[i].long_name = option[1] == '|' ? strndup(option + 2, strcspn(option + 2, ":?")) : NULL;
        joptions[i].has_value = strcspn(option, ":") != strlen(option);
        joptions[i].is_optional = strcspn(option, "?") != strlen(option);
        joptions[i].idx = i;
    }

    for (i32 i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg[0] == '-') {
            if (arg[1] == '-') {
                bool found = false;
                for (i32 j = 0; j < option_count; j++) {
                    if (joptions[j].long_name != NULL && strcmp(arg + 2, joptions[j].long_name) == 0) {
                        found = true;
                        if (joptions[j].has_value) {
                            if (i + 1 < argc && argv[i + 1][0] != '-') {
                                parsed[joptions[j].idx] = argv[++i];
                            }
                            else if (!joptions[j].is_optional) {
                                Console.error("Missing value for option: %s", arg);
                                return 1;
                            }
                            else {
                                parsed[joptions[j].idx] = strdup("");
                            }
                        }
                        else {
                            parsed[joptions[j].idx] = strdup("");
                        }
                        break;
                    }
                }
                if (!found) {
                    Console.warn("Unknown option: %s", arg);
                }
            }
            else {
                bool found = false;
                for (i32 j = 0; j < option_count; j++) {
                    if (arg[1] == joptions[j].short_name) {
                        found = true;
                        if (joptions[j].has_value) {
                            if (i + 1 < argc && argv[i + 1][0] != '-') {
                                parsed[joptions[j].idx] = argv[++i];
                            }
                            else if (!joptions[j].is_optional) {
                                Console.error("Missing value for option: %s", arg);
                                return 1;
                            }
                            else {
                                parsed[joptions[j].idx] = strdup("");
                            }
                        }
                        else {
                            parsed[joptions[j].idx] = strdup("");
                        }
                        break;
                    }
                }
                if (!found) {
                    Console.warn("Unknown option: %s", arg);
                }
            }
        }
        else {
            // Console.warn("Value without option: %s", arg);
        }
    }

    for (i32 i = 0; i < option_count; i++) {
        free(joptions[i].long_name);
    }

    return 0;
}

#endif // __UTILS_OPTIONS_H
