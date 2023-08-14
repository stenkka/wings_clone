#include "new.h"

#ifndef _LINKED_LIST_
#define _LINKED_LIST_

struct projectileNode {
    struct Circle projectile;
    struct projectileNode *next;
};

typedef struct projectileNode prNode_t;

prNode_t *createProjectileNode(prNode_t *head);
void prependProjectileNode(prNode_t **head, prNode_t *newNode);
void appendProjectileNode(prNode_t **head, prNode_t *newNode);
void freeProjectileNodes(prNode_t **head);

#endif