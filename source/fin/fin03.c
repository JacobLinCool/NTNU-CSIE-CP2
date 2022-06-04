#include "lib/catcher.h"
#include "lib/cimple/all.h"
#include "lib/uds/deque.h"
#include "lib/uds/vector.h"
#include "maze.h"

typedef char* string;

StructDeque(Deque, sNode*, NULL);
StructVector(NodeList, sNode*, NULL);
StructVector(Lists, NodeList*, NULL);

enum { ENTRANCE = 0, EXIT = 1, JUST_NODE = 2, VISITED_MODE = 3 };

i32 main(i32 argc, string argv[]) {
    setup_catcher();

    sNode* start = maze_gen();
    sNode* end = NULL;

    Deque* deq = create_Deque();
    deq->push(deq, start);

    size_t max_id = start->id;
    while (deq->size > 0) {
        sNode* curr = deq->pop(deq);
        if (curr->id > max_id) {
            max_id = curr->id;
        }
        if (curr->flag == EXIT) {
            end = curr;
            break;
        }

        struct list_head* iter;
        list_for_each(iter, &(curr->next_node_list)) {
            sNodeAddress* addr = list_entry(iter, sNodeAddress, list);
            if (addr->pNode->flag == JUST_NODE) {
                addr->pNode->flag = VISITED_MODE;
                deq->push(deq, addr->pNode);
            } else if (addr->pNode->flag == EXIT) {
                deq->push(deq, addr->pNode);
            }
        }
    }

    Lists* pathes = create_Lists();
    for (size_t i = 0; i < max_id; i++) {
        pathes->push(pathes, NULL);
    }
    pathes->data[start->id] = create_NodeList();
    pathes->data[start->id]->push(pathes->data[start->id], start);

    Deque* deq2 = create_Deque();
    deq2->push(deq2, start);

    while (deq2->size > 0) {
        sNode* curr = deq2->pop(deq2);
        if (curr->flag == EXIT) {
            break;
        }

        struct list_head* iter;
        list_for_each(iter, &(curr->next_node_list)) {
            sNodeAddress* addr = list_entry(iter, sNodeAddress, list);

            if (addr->pNode->flag != ENTRANCE) {
                if (addr->pNode->flag == VISITED_MODE) {
                    addr->pNode->flag = ENTRANCE;
                }

                deq2->push(deq2, addr->pNode);
                pathes->data[addr->pNode->id] =
                    pathes->data[curr->id]->clone(pathes->data[curr->id]);
                pathes->data[addr->pNode->id]->push(pathes->data[addr->pNode->id], addr->pNode);
            }
        }
    }

    NodeList* path = pathes->data[end->id];
    for (size_t i = 0; i < path->size; i++) {
        printf("%d(%s)", path->data[i]->id, path->data[i]->name);

        if (i != path->size - 1) {
            printf(" -> ");
        }
    }
    printf("\n");

    return EXIT_SUCCESS;
}
