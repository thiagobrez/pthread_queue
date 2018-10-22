#ifndef QUEUE
#define QUEUE

#include "line.h"

typedef struct {
    Line* lines[3];
    // Line* lines;
    int size;
} Queue;

#endif