#include "helper.h"
#include "keywords.h"
#include "lib/string.h"

Language get_language(string str) {
    Language lang = unknown_language;

    for (i32 i = 0; i < language_count; i++) {
        if (strcmp(str, languages[i]) == 0) {
            lang = (Language)i;
            break;
        }
    }

    return lang;
}

bool is_number(string str) {
    i64 i;
    f128 f;

    if (sscanf(str, "%" SCNd64, &i) == 1) {
        return true;
    }
    if (sscanf(str, "%Lf", &f) == 1) {
        return true;
    }

    return false;
}

string lower(string str) {
    string result = calloc(strlen(str) + 1, sizeof(char));
    for (i32 i = 0; i < strlen(str); i++) {
        result[i] = tolower(str[i]);
    }
    return result;
}

void print_help(i32 argc, string argv[]) {
    Console.rainbow("Jacob's Source Code Viewer.");
    Console.info("Options:");
    Console.info("    -n, --linenum             : print line numbers");
    Console.info("    -c, --color               : colorize output");
    Console.info("    -l, --language <language> : specify language (default: \"c\")");
    Console.info("Supported languages:");
    for (i32 i = 0; i < language_count; i++) {
        Console.info("    %-16s " __FMT_GRAY "(%3d keywords)" __FMT_NONE, languages[i], keyword_sizes[i]);
    }
    Console.info("Example:");
    Console.info(__FMT_YELLOW "    %s --linenum --color --language javascript path/to/source.js" __FMT_NONE, argv[0]);
    Console.info("Licensing:");
    Console.info("    Copyright (c) 2022 Jacob Lin <hi@jacoblin.cool>");
    Console.info("    Distributed under the MIT License (MIT)");
    Console.info("    Source code is available on GitHub (maybe)");
    Console.info("    Application Built on " __DATE__ " " __TIME__);
}

i32 main(i32 argc, string argv[]) {
    if (argc < 2) {
        Console.error("Usage: %s [options] <source>", argv[0]);
        print_help(argc, argv);
        return EXIT_FAILURE;
    }

    string options[] = { "n|linenum?", "c|color?", "l|language:?" };
    string parsed[3] = { NULL };
    if (parse_options(argc, argv, options, 3, parsed)) {
        Console.error("Invalid arguments.");
        return EXIT_FAILURE;
    }

    bool show_number = parsed[0] != NULL;
    bool color = parsed[1] != NULL;

    if (parsed[2] == NULL) {
        parsed[2] = strdup("c");
    }

    Language language = get_language(lower(parsed[2]));
    if (language == unknown_language) {
        Console.error("Unknown language: %s", parsed[2]);
        return EXIT_FAILURE;
    }

    string single_comment = single_line_comment[language];
    u64 single_comment_len = strlen(single_comment);
    string comment_start = multi_line_comment[language][0];
    u64 comment_start_len = strlen(comment_start);
    string comment_end = multi_line_comment[language][1];
    u64 comment_end_len = strlen(comment_end);

    string filename = argv[argc - 1];
    string source = File.read(filename);
    if (source == NULL) {
        Console.error("Failed to read file: %s", filename);
        return EXIT_FAILURE;
    }

    string tag = "Parsing";
    Timing.start(tag);
    u64 source_len = strlen(source), offset = 0;
    u64 line_number = 1;
    bool in_comment = false, in_string = false, oneline_comment = false;
    char quote = '\0';
    string last_color = NULL;
    while (offset < source_len) {
        if (show_number) {
            string color = line_number % 20 == 0 ? __FMT_WHITE : __FMT_GRAY;
            printf("%s%4" PRIu64 __FMT_GRAY " |" __FMT_NONE, color, line_number++);
        }

        if (color && last_color != NULL) {
            printf("%s", last_color);
        }

        while (source[offset] != '\n' && offset < source_len) {
            if (source[offset] == '\r') {
                offset++;
                continue;
            }

            string token = calloc(16 * 1024, sizeof(char));
            // space and tab
            if (sscanf(source + offset, "%[ \t]", token) == 1) {
                string replaced = replace(token, "\t", "    ");
                printf("%s", replaced);
                free(replaced);
            }
            // brackets
            else if (strchr("{}[]()", source[offset])) {
                if (color && !in_comment && !in_string) {
                    printf(__FMT_CYAN);
                }
                token = strndup(source + offset, 1);
                printf("%s", token);
                if (color && !in_comment && !in_string) {
                    printf(__FMT_NONE);
                }
            }
            // string
            else if (strchr("\"\'", source[offset]) && (
                offset == 0 ||
                source[offset - 1] != '\\' ||
                (offset > 1 && source[offset - 1] == '\\' && source[offset - 2] == '\\')
                )) {
                if (color && !in_comment && !in_string) {
                    printf(__FMT_GREEN);
                }
                token = strndup(source + offset, 1);
                printf("%s", token);
                if (color && !in_comment && in_string && quote == source[offset]) {
                    printf(__FMT_NONE);
                }

                if (!in_comment && (quote == '\0' || quote == source[offset])) {
                    in_string = !in_string;
                    quote = in_string ? source[offset] : '\0';
                }
            }
            // escaped string
            else if (strchr("\"\'", source[offset])) {
                token = strndup(source + offset, 1);
                printf("%s", token);
            }
            // single-line comment
            else if (strncmp(source + offset, single_comment, single_comment_len) == 0) {
                if (color && !in_string) {
                    printf(__FMT_GRAY);
                }
                token = strndup(source + offset, 2);
                printf("%s", token);

                if (!in_string) {
                    in_comment = true;
                    oneline_comment = true;
                }
            }
            // multi-line comment start
            else if (comment_start && strncmp(source + offset, comment_start, comment_start_len) == 0) {
                if (color && !in_string && !in_comment) {
                    printf(__FMT_GRAY);
                    last_color = __FMT_GRAY;
                }
                token = strndup(source + offset, 2);
                printf("%s", token);

                if (!in_string) {
                    in_comment = true;
                }
            }
            // multi-line comment end
            else if (comment_end && strncmp(source + offset, comment_end, comment_end_len) == 0) {
                token = strndup(source + offset, 2);
                printf("%s", token);

                if (color && !in_string && in_comment && !oneline_comment) {
                    printf(__FMT_NONE);
                    last_color = NULL;
                }

                if (!oneline_comment && !in_string) {
                    in_comment = false;
                }
            }
            // operators
            else if (strchr("+-*/%&|^~!=<>", source[offset])) {
                if (color && !in_comment && !in_string) {
                    printf(__FMT_YELLOW);
                }
                token = strndup(source + offset, 1);
                printf("%s", token);
                if (color && !in_comment && !in_string) {
                    printf(__FMT_NONE);
                }
            }
            // ; and ,
            else if (strchr(";,", source[offset])) {
                if (color && !in_comment && !in_string) {
                    printf(__FMT_GRAY);
                }
                token = strndup(source + offset, 1);
                printf("%s", token);
                if (color && !in_comment && !in_string) {
                    printf(__FMT_NONE);
                }
            }
            // other things
            else if (sscanf(source + offset, "%[^] \t\n(){};,[\"\'+-*/%%&|^~!=<>]", token) == 1) {
                if (in_comment) {
                    printf("%s", token);
                }
                else if (in_string) {
                    printf("%s", token);
                }
                else if (is_keyword(language, token) != -1) {
                    if (color && !in_comment && !in_string) {
                        printf(__FMT_BLUE);
                    }
                    printf("%s", token);
                    if (color && !in_comment && !in_string) {
                        printf(__FMT_NONE);
                    }
                }
                else if (is_number(token)) {
                    if (color && !in_comment && !in_string) {
                        printf(__FMT_MAGENTA);
                    }
                    printf("%s", token);
                    if (color && !in_comment && !in_string) {
                        printf(__FMT_NONE);
                    }
                }
                else {
                    printf("%s", token);
                }
            }

            offset += strlen(token);
            free(token);
        }

        offset++;
        if (oneline_comment) {
            oneline_comment = false;
            in_comment = false;
            printf(__FMT_NONE);
        }
        printf("\n");
    }

    Console.info("Spent %0.1Lf ms to parse the source code.", Timing.check(tag));
    return EXIT_SUCCESS;
}

/**
 * @brief This is a test multi-line comment
 * Yes, it is a test
 */

int a = /* Test again */ 100; // Test int x = 0; /* Test again */ int y = 0;
