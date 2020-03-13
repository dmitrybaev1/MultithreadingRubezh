#include <stdlib.h>
#include <pthread.h>
#include "queue.h"

void enqueue(ConcurrentQueue* queue, void* value)
{
    pthread_mutex_lock( &(queue->mutex) );

    QueueNode* newNode = (QueueNode*) malloc( sizeof(QueueNode) );
    newNode->value = value;
    newNode->next = NULL;

    QueueNode* node = queue->first;
    if (node != NULL)
    {
        while (node->next != NULL) node = node->next;
        node->next = newNode;
    }
    else queue->first = newNode;

    pthread_mutex_unlock(&(queue->mutex));
}

void* dequeue(ConcurrentQueue* queue)
{
    void* result = NULL;

    pthread_mutex_lock( &(queue->mutex) );

    QueueNode* first = queue->first;
    if (first != NULL)
    {
        result = first->value;
        queue->first = first->next;
        free(first);
    }

    pthread_mutex_unlock(&(queue->mutex));

    return result;
}

