#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdbool.h>
#include "segel.h"

typedef struct node {
    int val;
    struct timeval current_time;
    struct node* next;
    struct node* prev;
} Node;

typedef struct queue {
	struct node* head;
	struct node* tail;
	int size;
}Queue;

//front = head-> ... ->tail = back

Node* node_create(int val);
void free_node(Node* to_delete);

Queue* queue_create ();
int queue_get_size (Queue* qu);
void queue_destroy(Queue* qu);
void queue_clear (Queue* qu);
bool queue_is_empty (Queue* qu);

void queue_pop(Queue* qu);
void queue_front (Queue* qu, int* val, struct timeval* arriv_time);

bool queue_push_back (Queue* qu, int val);
void queue_pop_back (Queue* qu);
void queue_drop_random (Queue* qu);
#endif
