/**
 * @author Bruno Henrique Bertozzo
 * @author João Grasel Cariolato
 * @author Thiago Brezinski
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include "queue.h"

int ID_COUNTER = 0;
int BALCONY_THREADS;
pthread_t simulator_thid, *balcony_thids;
pthread_mutex_t lock;

int random_priority(int max) {
    srand((unsigned) time(NULL));
    return (rand() % max) + 1;
}

void add_to_queue(Queue* queue, Node* node) {
    pthread_mutex_lock(&lock);

    if(queue->size == 0) {
        queue->lines[node->priority -1]->first = queue->lines[node->priority -1]->last = node;
    } else {
        queue->lines[node->priority -1]->last->next = (struct Node*) node;
        queue->lines[node->priority -1]->last = queue->lines[node->priority -1]->last->next;
    }

    queue->lines[node->priority -1]->size++;
    queue->size++;

    pthread_mutex_unlock(&lock);
}

void remove_from_queue(Queue* queue, int line) {
    pthread_mutex_lock(&lock);

    Node* deleted_node = queue->lines[line]->first;
    queue->lines[line]->first = queue->lines[line]->first->next;
    deleted_node->next = NULL;

    queue->lines[line]->size--;
    queue->size--;

    pthread_mutex_unlock(&lock);
}

void* simulator(void* args) {
    Queue* queue = args;

    balcony_thids = (pthread_t *) malloc(BALCONY_THREADS * sizeof(pthread_t));
    for(int i = 0; i < BALCONY_THREADS; i++) {
        pthread_create(&balcony_thids[i], NULL, balcony, NULL);
    }

    while(1) {
        Node* node = malloc(sizeof(Node));
        node->priority = random_priority(3);
        node->person = malloc(sizeof(Person));
    
        char string_priority[12], string_counter[12];
        sprintf(string_priority, "%d", node->priority);
        sprintf(string_counter, "%d", (ID_COUNTER + 1));

        char id[32] = strcat(string_priority, "P");
        id[32] = strcat(string_priority, string_counter);
        *node->person->id = id;

        add_to_queue(queue, node);

        sleep(3);
    }

    pthread_exit(NULL);
}

void* balcony(void* args) {
    Queue* queue = args;

    while(queue->size) {
        if(queue->lines[0]->size) {
            remove_from_queue(queue, 0);
        } else if(queue->lines[1]->size) {
            remove_from_queue(queue, 1);
        } else if(queue->lines[2]->size) {
            remove_from_queue(queue, 2);
        }
        sleep(5);
    }

    pthread_exit(NULL);
}

int main() {
    printf("Enter the number of balcony threads you want to create: ");
    scanf("%d", &BALCONY_THREADS);

   	pthread_mutex_init(&lock, NULL);

    Queue* queue = malloc(sizeof(Queue));
    queue->size = 0;

    for(int i = 0; i < 3; i++) {
        queue->lines[i] = malloc(sizeof(Line));
        queue->lines[i]->size = 0;
    }

    pthread_create(&simulator_thid, NULL, simulator, (void *) queue);

    pthread_join(simulator_thid, NULL);
    for(int i = 0; i < BALCONY_THREADS; i++) {
        pthread_join(balcony_thids[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    exit(0);
}