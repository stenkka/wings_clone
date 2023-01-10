#include <memory.h>
#include "linked_list.h"

prNode_t *createProjectileNode(prNode_t *head) {
    prNode_t *newNode = malloc(sizeof(prNode_t));
    newNode->next = NULL;
    return newNode;
}

void prependProjectileNode(prNode_t **head, prNode_t *newNode) {
    newNode->next = *head;
    *head = newNode;
}