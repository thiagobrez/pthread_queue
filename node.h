#ifndef NODE
#define NODE

#include "person.h"

typedef struct {
    Person* person;
    struct Node* next;
    int priority;
} Node;

#endif