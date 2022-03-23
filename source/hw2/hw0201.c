#include "hw0201.h"
#include "mymatch.h"

i32 main() {
    string* pList = NULL;
    i32 size = -100;

    size = mymatch(&pList, "abc", "*");
    assert(size == 1);
    assert(strcmp(pList[0], "abc") == 0);
    free(pList);

    size = mymatch(&pList, "abcdefg", "*abcd??*");
    assert(size == 1);
    assert(strcmp(pList[0], "abcdefg") == 0);
    free(pList);

    size = mymatch(&pList, "abcdefg", "*abcd*f");
    assert(size == 0);
    free(pList);

    size = mymatch(&pList, "abcdefg", "*abcd*f*");
    assert(size == 1);
    assert(strcmp(pList[0], "abcdefg") == 0);
    free(pList);

    size = mymatch(&pList, "abcdefg", "*a*b*c*d*e*f*g*");
    assert(size == 1);
    assert(strcmp(pList[0], "abcdefg") == 0);
    free(pList);

    size = mymatch(&pList, "abcdefg", "a**g");
    assert(size == 1);
    assert(strcmp(pList[0], "abcdefg") == 0);
    free(pList);

    size = mymatch(&pList, "abcdefg", "*a*g");
    assert(size == 1);
    assert(strcmp(pList[0], "abcdefg") == 0);
    free(pList);

    size = mymatch(&pList, "abc adc aec cfv bbc cdc acc", "a?c");
    assert(size == 4);
    assert(strcmp(pList[0], "abc") == 0);
    assert(strcmp(pList[1], "adc") == 0);
    assert(strcmp(pList[2], "aec") == 0);
    assert(strcmp(pList[3], "acc") == 0);
    free(pList);

    size = mymatch(&pList, "abc adc aec cfv bbc cdc acc", "a?c*");
    assert(size == 4);
    assert(strcmp(pList[0], "abc") == 0);
    assert(strcmp(pList[1], "adc") == 0);
    assert(strcmp(pList[2], "aec") == 0);
    assert(strcmp(pList[3], "acc") == 0);
    free(pList);

    size = mymatch(&pList, "xxxabcyyyabczzzdefabc abcabcabcdefdef defdefabcdefdeg", "*abc???def*");
    assert(size == 2);
    assert(strcmp(pList[0], "xxxabcyyyabczzzdefabc") == 0);
    assert(strcmp(pList[1], "abcabcabcdefdef") == 0);
    free(pList);

    size = mymatch(&pList, "aa  aa", "*");
    assert(size == 2);
    assert(strcmp(pList[0], "aa") == 0);
    assert(strcmp(pList[1], "aa") == 0);
    free(pList);

    return 0;
}
