#include "queue.h"

#include <stdlib.h>

QueueNode* queue_create_node(void* value)
{
	QueueNode* node = malloc(sizeof(QueueNode));
	node->value = value;
	node->next = NULL;

	return node;
}

Queue* queue_create()
{
	Queue* q = malloc(sizeof(Queue));
	q->head = queue_create_node(NULL);
	q->tail = q->head;
	pthread_mutex_init(&q->head_mutex, NULL);
	pthread_mutex_init(&q->tail_mutex, NULL);
	return q;
}

void queue_destroy(Queue* q)
{
	QueueNode* node = q->head->next;
	while (node != NULL) {
		QueueNode* next = node->next;
		free(node);
		node = next;
	}
	free(q->head);
	free(q->tail);
	pthread_mutex_destroy(&q->head_mutex);
	pthread_mutex_destroy(&q->tail_mutex);
	free(q);
}

void enqueue(Queue* q, void* value)
{
	QueueNode* node = queue_create_node(value);
	pthread_mutex_lock(&q->tail_mutex);
	q->tail->next = node;
	q->tail = node;
	pthread_mutex_unlock(&q->tail_mutex);
}

void* dequeue(Queue* q)
{
	pthread_mutex_lock(&q->head_mutex);
	QueueNode* head = q->head;
	if (head->next == NULL) {
		pthread_mutex_unlock(&q->head_mutex);
		return NULL;
	}
	void* value = head->next->value;
	q->head = head->next;
	pthread_mutex_unlock(&q->head_mutex);
	return value;
}
