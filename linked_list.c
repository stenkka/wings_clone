#include <memory.h>
#include "linked_list.h"

prNode_t *createProjectileNode(prNode_t *head) {
    prNode_t *newNode = malloc(sizeof(prNode_t));
    newNode->next = NULL;
    return newNode;
}

// Add a new node to the front of the list
void prependProjectileNode(prNode_t **head, prNode_t *newNode) {
    newNode->next = *head;
    *head = newNode;
}

// Add a new node to the end of the list
void appendProjectileNode(prNode_t **head, prNode_t *newNode) {
    prNode_t **tmp = head;
    // Traverse nodes until we are past the last node (*tmp == NULL)
    while (*tmp) {
        tmp = &(*tmp)->next;
    }
    // append the new node
    newNode->next = *tmp;
    *tmp = newNode;
}