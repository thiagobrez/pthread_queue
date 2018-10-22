#ifndef NODE
#define NODE

#include "person.h"

typedef struct Node {
    Person* person;
    struct Node* next;
    int priority;
} Node;

#endif