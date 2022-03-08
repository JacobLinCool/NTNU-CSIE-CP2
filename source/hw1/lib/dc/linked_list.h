#include "../base.h"

#define LinkedNode(name, type) \
    typedef struct name { \
        type val; \
        struct name* next; \
    } name; \

#define DoubleLinkedList(name, type) \
    typedef struct name { \
        type val; \
        struct name* prev; \
        struct name* next; \
    } name; \
