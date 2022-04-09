#include "mystring.h"

void test(string str, string delim);

i32 main() {
    test("abc def", " ");
    test("abc def ghi jkl", " ");
    test("abc def ghi jkl", "abc");
    test("abc def ghi jkl", "jkl");
    test("abc def ghi jkl", "");
    test("abc def ghi jkl", "xyz");
    test("abc def ghi jkl", "abc def ghi jkl");
    test("abc def ghi jkl", "abc def ghi jkl ");

    printf(CLR_GREEN "Done.\n" CLR_NONE);

    return EXIT_SUCCESS;
}

void test(string str, string delim) {
    string my_test_str = strdup(str);
    string std_test_str = strdup(str);

    string my_delim = strdup(delim);
    string std_delim = strdup(delim);

    char* my_saveptr = NULL;
    char* std_saveptr = NULL;

    string my_token = mystrtok_r(my_test_str, my_delim, &my_saveptr);
    string std_token = strtok_r(std_test_str, std_delim, &std_saveptr);
    while (my_token != NULL && std_token != NULL) {
        assert(strcmp(my_token, std_token) == 0);
        assert(my_saveptr - my_test_str == std_saveptr - std_test_str);
        my_token = mystrtok_r(NULL, my_delim, &my_saveptr);
        std_token = strtok_r(NULL, std_delim, &std_saveptr);
    }

    free(my_test_str), free(std_test_str), free(my_delim), free(std_delim);
}
