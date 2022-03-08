// #define DEBUG
#include "hw0105.h"

i32 main() {
    string* ppList = NULL;
    i32 counter = 0;
    string str = "abc:|:defgh:|:ijkl:mno:|:pq|rst:|:uvwxyz:|:";
    string separator = ":|:";

    i32 success = mystrsplit(&ppList, &counter, str, separator);

    assert(success == 0);
    assert(counter == 6);
    assert(strcmp(ppList[0], "abc") == 0);
    assert(strcmp(ppList[1], "defgh") == 0);
    assert(strcmp(ppList[2], "ijkl:mno") == 0);
    assert(strcmp(ppList[3], "pq|rst") == 0);
    assert(strcmp(ppList[4], "uvwxyz") == 0);
    assert(strcmp(ppList[5], "") == 0);
    DBG_PRINT("\n%s\n", format_array(str, ppList, 0, counter - 1));

    success = mystrsplit(NULL, &counter, str, separator);
    assert(success == -1);
    assert(counter == 6);
    success = mystrsplit(&ppList, NULL, str, separator);
    assert(success == -1);
    assert(ppList == NULL);
    assert(counter == 6);
    success = mystrsplit(&ppList, &counter, NULL, separator);
    assert(success == -1);
    assert(ppList == NULL);
    assert(counter == 6);
    success = mystrsplit(&ppList, &counter, str, "");
    assert(success == -1);
    assert(ppList == NULL);
    assert(counter == 6);

    success = mystrsplit(&ppList, &counter, "", "abc");
    assert(success == 0);
    assert(counter == 1);
    assert(strcmp(ppList[0], "") == 0);
    DBG_PRINT("\n%s\n", format_array(str, ppList, 0, counter - 1));

    success = mystrsplit(&ppList, &counter, "1,,2,,3,,4,,5", ",");
    assert(success == 0);
    assert(counter == 9);
    assert(strcmp(ppList[0], "1") == 0);
    assert(strcmp(ppList[1], "") == 0);
    assert(strcmp(ppList[2], "2") == 0);
    assert(strcmp(ppList[3], "") == 0);
    assert(strcmp(ppList[4], "3") == 0);
    assert(strcmp(ppList[5], "") == 0);
    assert(strcmp(ppList[6], "4") == 0);
    assert(strcmp(ppList[7], "") == 0);
    assert(strcmp(ppList[8], "5") == 0);
    DBG_PRINT("\n%s\n", format_array(str, ppList, 0, counter - 1));

    success = mystrsplit(&ppList, &counter, "abc", "abc");
    assert(success == 0);
    assert(counter == 2);
    assert(strcmp(ppList[0], "") == 0);
    assert(strcmp(ppList[1], "") == 0);
    DBG_PRINT("\n%s\n", format_array(str, ppList, 0, counter - 1));

    success = mystrsplit(&ppList, &counter, "12\n  \t 234      45", NULL);
    assert(success == 0);
    assert(counter == 3);
    DBG_PRINT("\n%s\n", format_array(str, ppList, 0, counter - 1));
    assert(strcmp(ppList[0], "12") == 0);
    assert(strcmp(ppList[1], "234") == 0);
    assert(strcmp(ppList[2], "45") == 0);

    printf("Passed!\n");

    return 0;
}
