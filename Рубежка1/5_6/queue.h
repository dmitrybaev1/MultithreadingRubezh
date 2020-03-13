#include <pthread.h>

typedef struct QueueNode {
	void* value;
	struct QueueNode* next;
} QueueNode;

typedef struct Queue {
	QueueNode* head;
	QueueNode* tail;
    pthread_mutex_t head_mutex;
    pthread_mutex_t tail_mutex;
} Queue;

Queue* queue_create();
void queue_destroy(Queue*);
void enqueue(Queue*, void*);
void* dequeue(Queue*);
