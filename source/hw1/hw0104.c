// #define DEBUG
#include "hw0104.h"

u8 test_mystrchr();
u8 test_mystrrchr();
u8 test_mystrspn();
u8 test_mystrcspn();
u8 test_mystrpbrk();
u8 test_mystrstr();
u8 test_mystrtok();

string str = NULL;

i32 main() {
    str = (string)malloc(sizeof(char) * 100);
    strcpy(str, "abcdefghijklmnopqrstuvwxyz");

    u8 failed;
    failed = 0;
    printf(CLR_CYAN "Testing mystrchr()... " CLR_NONE);
    failed += test_mystrchr('a');
    failed += test_mystrchr('z');
    failed += test_mystrchr('\0');
    if (!failed) {
        printf(CLR_GREEN "Passed!\n" CLR_NONE);
    }
    else {
        printf(CLR_RED "Failed!\n" CLR_NONE);
    }

    failed = 0;
    printf(CLR_CYAN "Testing mystrrchr()... " CLR_NONE);
    failed += test_mystrrchr('a');
    failed += test_mystrrchr('z');
    failed += test_mystrrchr('1');
    failed += test_mystrrchr('\0');
    if (!failed) {
        printf(CLR_GREEN "Passed!\n" CLR_NONE);
    }
    else {
        printf(CLR_RED "Failed!\n" CLR_NONE);
    }

    failed = 0;
    printf(CLR_CYAN "Testing mystrspn()... " CLR_NONE);
    failed += test_mystrspn("gfedcba");
    failed += test_mystrspn("abcdefghijklmnopqrstuvwxyz");
    failed += test_mystrspn("");
    if (!failed) {
        printf(CLR_GREEN "Passed!\n" CLR_NONE);
    }
    else {
        printf(CLR_RED "Failed!\n" CLR_NONE);
    }

    failed = 0;
    printf(CLR_CYAN "Testing mystrcspn()... " CLR_NONE);
    failed += test_mystrcspn("hijklmn");
    failed += test_mystrcspn("abcdefghijklmnopqrstuvwxyz");
    failed += test_mystrcspn("");
    if (!failed) {
        printf(CLR_GREEN "Passed!\n" CLR_NONE);
    }
    else {
        printf(CLR_RED "Failed!\n" CLR_NONE);
    }

    failed = 0;
    printf(CLR_CYAN "Testing mystrpbrk()... " CLR_NONE);
    failed += test_mystrpbrk("fged");
    failed += test_mystrpbrk("abcdefghijklmnopqrstuvwxyz");
    failed += test_mystrpbrk("");
    if (!failed) {
        printf(CLR_GREEN "Passed!\n" CLR_NONE);
    }
    else {
        printf(CLR_RED "Failed!\n" CLR_NONE);
    }

    failed = 0;
    printf(CLR_CYAN "Testing mystrstr()... " CLR_NONE);
    failed += test_mystrstr("klmn");
    failed += test_mystrstr("abcdefghijklmnopqrstuvwxyz");
    failed += test_mystrstr("");
    if (!failed) {
        printf(CLR_GREEN "Passed!\n" CLR_NONE);
    }
    else {
        printf(CLR_RED "Failed!\n" CLR_NONE);
    }

    failed = 0;
    printf(CLR_CYAN "Testing mystrtok()... " CLR_NONE);
    failed += test_mystrtok("1");
    failed += test_mystrtok("");
    failed += test_mystrtok("def");
    failed += test_mystrtok("abh");
    failed += test_mystrtok("abc");
    failed += test_mystrtok("xyz");
    failed += test_mystrtok("abcdefghijklmnopqrstuvwxyz");
    if (!failed) {
        printf(CLR_GREEN "Passed!\n" CLR_NONE);
    }
    else {
        printf(CLR_RED "Failed!\n" CLR_NONE);
    }

    printf(CLR_MAGENTA "All Tests Done!\n" CLR_NONE);

    return 0;
}

u8 test_mystrchr(i32 chr) {
    char* my_ans = mystrchr(str, chr);
    char* ans = strchr(str, chr);
    if (my_ans != ans) {
        printf(CLR_YELLOW "Test Failed: mystrchr | chr = %" PRId32 CLR_NONE "\n", chr);
        printf(CLR_GREEN "[EXPECTED]" CLR_NONE " value = %p\n", ans);
        printf(CLR_RED "[RECEIVED]" CLR_NONE " value = %p\n", my_ans);
        // exit(EXIT_FAILURE);
        return 1;
    }
    return 0;
}
u8 test_mystrrchr(i32 chr) {
    char* my_ans = mystrrchr(str, chr);
    char* ans = strrchr(str, chr);
    if (my_ans != ans) {
        printf(CLR_YELLOW "Test Failed: mystrrchr | chr = %" PRId32 CLR_NONE "\n", chr);
        printf(CLR_GREEN "[EXPECTED]" CLR_NONE " value = %p\n", ans);
        printf(CLR_RED "[RECEIVED]" CLR_NONE " value = %p\n", my_ans);
        // exit(EXIT_FAILURE);
        return 1;
    }
    return 0;
}
u8 test_mystrspn(string accept) {
    i32  my_ans = mystrspn(str, accept);
    i32 ans = strspn(str, accept);
    if (my_ans != ans) {
        printf(CLR_YELLOW "Test Failed: mystrspn | accept = \"%s\"\n", accept);
        printf(CLR_GREEN "[EXPECTED]" CLR_NONE " value = %" PRId32 "\n", ans);
        printf(CLR_RED "[RECEIVED]" CLR_NONE " value = %" PRId32 "\n", my_ans);
        // exit(EXIT_FAILURE);
        return 1;
    }
    return 0;
}
u8 test_mystrcspn(string reject) {
    i32  my_ans = mystrcspn(str, reject);
    i32 ans = strcspn(str, reject);
    if (my_ans != ans) {
        printf(CLR_YELLOW "Test Failed: mystrcspn | reject = \"%s\"\n", reject);
        printf(CLR_GREEN "[EXPECTED]" CLR_NONE " value = %" PRId32 "\n", ans);
        printf(CLR_RED "[RECEIVED]" CLR_NONE " value = %" PRId32 "\n", my_ans);
        // exit(EXIT_FAILURE);
        return 1;
    }
    return 0;
}
u8 test_mystrpbrk(string accept) {
    char* my_ans = mystrpbrk(str, accept);
    char* ans = strpbrk(str, accept);
    if (my_ans != ans) {
        printf(CLR_YELLOW "Test Failed: mystrpbrk | accept = \"%s\"\n", accept);
        printf(CLR_GREEN "[EXPECTED]" CLR_NONE " value = %p\n", ans);
        printf(CLR_RED "[RECEIVED]" CLR_NONE " value = %p\n", my_ans);
        // exit(EXIT_FAILURE);
        return 1;
    }
    return 0;
}
u8 test_mystrstr(string needle) {
    char* my_ans = mystrstr(str, needle);
    char* ans = strstr(str, needle);
    if (my_ans != ans) {
        printf(CLR_YELLOW "Test Failed: mystrstr | needle = \"%s\"\n", needle);
        printf(CLR_GREEN "[EXPECTED]" CLR_NONE " value = %p\n", ans);
        printf(CLR_RED "[RECEIVED]" CLR_NONE " value = %p\n", my_ans);
        // exit(EXIT_FAILURE);
        return 1;
    }
    return 0;
}
u8 test_mystrtok(string delim) {
    string str1 = (string)malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(str1, str);
    string str2 = (string)malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(str2, str);

    string ans = strtok(str1, delim);
    string my_ans = mystrtok(str2, delim);
    if (ans == NULL && my_ans == NULL) {
        return 0;
    }
    if ((ans == NULL ^ my_ans == NULL) || (strcmp(ans, my_ans) != 0)) {
        printf(CLR_YELLOW "Test Failed: mystrtok | delim = \"%s\"\n", delim);
        printf(CLR_GREEN "[EXPECTED]" CLR_NONE " value = %p [%s]\n", ans, ans);
        printf(CLR_RED "[RECEIVED]" CLR_NONE " value = %p [%s]\n", my_ans, my_ans);
        // exit(EXIT_FAILURE);
        return 1;
    }
    return 0;
}
