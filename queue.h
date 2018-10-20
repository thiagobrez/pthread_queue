#ifndef QUEUE
#define QUEUE

#include "person.h"

typedef struct {
    Person* first;
    Person* last;
    Person* lines;
    int size;
} Queue;

#endif