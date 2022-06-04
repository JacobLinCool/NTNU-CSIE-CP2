#include "maze.h"

#include <stdlib.h>
#include <string.h>

// Node1 (start) --> Node2 (Exit)
sNode *maze_gen(void) {
    // Node Generation
    sNode *pNode1 = malloc(sizeof(sNode));

    pNode1->id = 1;
    pNode1->flag = 0;
    strncpy(pNode1->name, "Welcome", 32);
    pNode1->next_node_list.prev = &(pNode1->next_node_list);
    pNode1->next_node_list.next = &(pNode1->next_node_list);

    sNode *pNode2 = malloc(sizeof(sNode));

    pNode2->id = 1;
    pNode2->flag = 1;
    strncpy(pNode2->name, "End", 32);
    pNode2->next_node_list.prev = &(pNode2->next_node_list);
    pNode2->next_node_list.next = &(pNode2->next_node_list);

    // Unidirectional Edge Generation
    // Node1 -> Node2
    sNodeAddress *pEdge1 = malloc(sizeof(sNodeAddress));
    pEdge1->pNode = pNode2;
    list_add_tail(&(pEdge1->list), &(pNode1->next_node_list));

    // Return
    return pNode1;
}