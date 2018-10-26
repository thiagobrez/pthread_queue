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
#include <string.h>
#include "queue.h"

int ID_COUNTER = 0;
int BALCONY_THREADS[1];
pthread_t simulator_thid, *balcony_thids;
pthread_mutex_t lock;

int random_priority(int max) {
    srand((unsigned) time(NULL));
    return (rand() % max) + 1;
}

void add_to_queue(Queue* queue, Node* node) {
    printf("Simulator waiting to add \n");

    pthread_mutex_lock(&lock);

    printf("Simulator entered CS \n");
    if(queue->lines[node->priority -1]->size == 0) {
        queue->lines[node->priority -1]->first = node;
        queue->lines[node->priority -1]->last = node;
    } else {
        // queue->lines[node->priority -1]->last->next = (struct Node*) node;
        queue->lines[node->priority -1]->last->next = node;
        queue->lines[node->priority -1]->last = queue->lines[node->priority -1]->last->next;
    }

    queue->lines[node->priority - 1]->size++;
    queue->size++;

    printf("Simulator created Person with id %i and priority %i \n", node->person->id, node->priority);

    printf("Simulator left CS \n");
    pthread_mutex_unlock(&lock);
}

void remove_from_queue(Queue* queue) {
    printf("Balcony %i waiting to remove \n", (int) pthread_self());
    pthread_mutex_lock(&lock);
    printf("Balcony %i entered CS \n", (int) pthread_self());

    int line = 0;

    if(queue->size) {
        if(queue->lines[0]->size) {
            line = 0;
        } else if(queue->lines[1]->size) {
            line = 1;
        } else if(queue->lines[2]->size) {
            line = 2;
        }
    } else {
        printf("Balcony %i left CS doing nothing \n", (int) pthread_self());
        pthread_mutex_unlock(&lock);
        return;
    }

    Node* deleted_node = queue->lines[line]->first;
    queue->lines[line]->first = queue->lines[line]->first->next;
    deleted_node->next = NULL;
    printf("Balcony %i deleted Person with id %i from priority %i \n", (int) pthread_self(), deleted_node->person->id, deleted_node->priority);
    free(deleted_node);

    queue->lines[line]->size--;
    queue->size--;

    if(queue->lines[line]->size == 0) queue->lines[line]->last = NULL;

    printf("Balcony %i left CS \n", (int) pthread_self());
    pthread_mutex_unlock(&lock);
}

void* balcony(void* args) {
    printf("Created balcony thread with id %i \n", (int) pthread_self());

    Queue* queue = args;

    while(1) {
        remove_from_queue(queue);
        printf("Balcony %i is sleeping \n", (int) pthread_self());
        sleep(10);
    }

    pthread_exit(NULL);
}

Person* create_person() {
    Person* person = malloc(sizeof(Person));
    ID_COUNTER++;
    person->id = ID_COUNTER;

    return person;
}

Node* create_node() {
    Node* node = malloc(sizeof(Node));
    node->priority = random_priority(3);
    node->person = create_person();
    node->next = NULL;

    return node;
}

void* simulator(void* args) {
    printf("Created simulator thread with id %i \n", (int) pthread_self());
    Queue* queue = args;

    while(1) {
        Node* node = create_node();
        add_to_queue(queue, node);
        sleep(1);
    }

    pthread_exit(NULL);
}

Line* create_line() {
    Line* line = malloc(sizeof(Line));

    line->first = NULL;
    line->last = NULL;
    line->size = 0;

    return line;
}

Queue* create_queue() {
   Queue* queue = malloc(sizeof(Queue));
   queue->size = 0;

    for(int i = 0; i < 3; i++) {
        queue->lines[i] = create_line();
    }

   return queue;
}

int main() {
    printf("Enter the number of balcony threads you want to create: ");
    fflush(stdin);
    scanf("%d", &BALCONY_THREADS[0]);

   	pthread_mutex_init(&lock, NULL);

    Queue* queue = create_queue();

    pthread_create(&simulator_thid, NULL, simulator, (void *) queue);
    balcony_thids = calloc(BALCONY_THREADS[0], sizeof(pthread_t));
    for(int i = 0; i < BALCONY_THREADS[0]; i++) {
        pthread_create(&balcony_thids[i], NULL, balcony, (void *) queue);
    }

    pthread_join(simulator_thid, NULL);
    for(int i = 0; i < BALCONY_THREADS[0]; i++) {
        pthread_join(balcony_thids[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    exit(0);
}