#include <stdlib.h>
#include <pthread.h>
#include "myheader.h"
#include <stddef.h>
#include <assert.h>
void enqueue(ConcurrentQueue* queue, Task* value)
{
    pthread_mutex_lock( &(queue->mutex) );
    QueueNode* newNode = (QueueNode*) malloc( sizeof(QueueNode) );
    newNode->value = value;
    newNode->next = NULL;
    QueueNode* node = queue->first;
    if (node != NULL)
    {
        while (node->next != NULL) {
            node = node->next;
        }
        node->next = newNode;
    }
    else{
        queue->first = newNode;
    }
    pthread_mutex_unlock(&(queue->mutex));
}

Task* dequeue(ConcurrentQueue* queue)
{
    pthread_mutex_lock( &(queue->mutex) );
    Task* result = NULL;


    QueueNode* first = queue->first;
    if (first != NULL)
    {
        result = first->value;
        queue->first = first->next;
        free(first);
    }

    pthread_mutex_unlock(&(queue->mutex));
    //assert(result==NULL);
    return result;
}

