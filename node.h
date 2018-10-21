#ifndef NODE
#define NODE

#include "person.h"

typedef struct {
    Person* person;
    Node* next;
    int priority;
} Node;

#endif