#include "queue.h"


Node* node_create(int val)
{
	Node* new_node = malloc(sizeof(*new_node));
	if (new_node == NULL)
	{
		return NULL;
	}
	new_node->val = val;
  	gettimeofday(&(new_node->current_time), NULL);
	new_node->next = NULL;
	new_node->prev = NULL;
	return new_node;
}

void free_node(Node* to_delete)
{
	if (to_delete == NULL)
	{
		return;
	}
	//free(to_delete->val);
	free(to_delete);
}

Queue* queue_create ()
{
	Queue* qu = malloc(sizeof(*qu));
	if (qu == NULL)
	{
		return NULL;
	}
	qu->size = 0;
	qu->head = NULL;
	qu->tail = NULL;
	return qu;
}

void queue_remove_node (Queue* qu, Node* node)
{
	if (qu == NULL || node == NULL)
	{
		return;
	}
	if (node == qu->head)
	{
		qu->head = node->next;
	}
	if (node == qu->tail)
	{
		qu->tail = node->prev;
	}
	if (node->prev != NULL)
	{
		Node* prev = node->prev;
		prev->next = node->next;
	}
	if (node->next != NULL)
	{
		Node* next = node->next;
		next->prev = node->prev;
	}
	qu->size--;
	free_node(node);
}

int queue_get_size (Queue* qu)
{
	if (qu == NULL)
	{
		return -1;
	}
	return qu->size;
}

void queue_destroy(Queue* qu)
{
	if (qu == NULL)
	{
		return;
	}
	queue_clear(qu);
	free(qu);
}

void queue_clear (Queue* qu)
{
	while (!queue_is_empty(qu))
	{
		queue_pop(qu);
	}
}

bool queue_is_empty (Queue* qu)
{
	if (qu == NULL)
	{
		return true;
	}
	return (qu->size == 0);
}

bool queue_push_back (Queue* qu, int val)
{
	if (qu == NULL)
	{
		return false;
	}
	Node* to_add = node_create(val);
	if (to_add == NULL)
	{
		return false;
	}
	if (queue_is_empty(qu))
	{
		qu->head = to_add;
		qu->tail = to_add;
		qu->size = 1;
	}
	else
	{
		Node* old_tail = qu->tail;
		qu->tail = to_add;
		old_tail->next = to_add;
		to_add->prev = old_tail;
		qu->size++;
	}
	return true;
}

void queue_front (Queue* qu, int* val, struct timeval* arrive_time)
{
	if (qu == NULL || qu->size == 0 || arrive_time == NULL)
	{
		return;
	}
	*val = qu->head->val;
	arrive_time->tv_sec = qu->head->current_time.tv_sec;
	arrive_time->tv_usec = qu->head->current_time.tv_usec;
}
void queue_pop (Queue* qu)
{
	if (qu == NULL || qu->size == 0)
	{
		return ;
	}
	queue_remove_node(qu, qu->head);
}

void queue_pop_back (Queue* qu)
{
	if (qu == NULL || qu->size == 0)
	{
		return ;
	}
	queue_remove_node(qu, qu->tail);
}

void queue_drop_random (Queue* qu)
{
	if (qu == NULL)
	{
		return;
	}
	int amount_to_drop = 0.3 * qu->size;
	for (int i = 0; i < amount_to_drop; i++)
	{
		int to_drop = rand() % qu->size;
		Node* temp = qu->head; 
		for (int j = 0; temp != NULL && j < to_drop; j++, temp = temp->next)
		{;	}
		queue_remove_node(qu, temp);
	}
}