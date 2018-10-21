#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include "queue.h"

int MAX_QUEUE_SIZE = 30;
pthread_t simulator_thid;
pthread_mutex_t lock;

void* simulator(void* args) {
    Queue* queue = args;

    while(queue->size < MAX_QUEUE_SIZE) {
        pthread_mutex_lock(&lock);
        //add to queue
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main() {
    Queue* queue = malloc(sizeof(Queue));
    queue->size = 0;

    for(int i = 0; i < 3; i++) {
        queue->lines[i] = malloc(sizeof(Line));
        queue->lines[i]->size = 0;
    }

    pthread_create(&simulator_thid, NULL, simulator, (void *) queue);
    //create thread to balcony
    //create thread to aging

    pthread_join(simulator_thid, NULL);
    // pthread_join(balcony_thid, NULL);
    // pthread_join(aging_thid, NULL);
}