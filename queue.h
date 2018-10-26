#ifndef QUEUE
#define QUEUE

#include "line.h"

typedef struct {
    Line* lines[3];
    int size;
} Queue;

#endif