#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include "queue.h"

int MAX_QUEUE_SIZE = 10;
pthread_t simulator_thid;
pthread_mutex_t lock;

void* simulator(void* args) {
    while(1) {
        pthread_mutex_lock(&lock);
        //add to queue
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main() {
    Queue* queue = malloc(sizeof(Queue));
    queue -> first = 0;
    queue -> last = 0;
    queue -> size = 0;
    queue -> lines = calloc(MAX_QUEUE_SIZE, sizeof (Person));

    pthread_create(&simulator_thid, NULL, simulator, (void *) queue);
    //create thread to balcony
    //create thread to aging
}