#include <pthread.h>
typedef struct QueueNode {
    void* value;
    struct QueueNode* next;
} QueueNode;

typedef struct ConcurrentQueue{
    QueueNode* first;
    pthread_mutex_t mutex;
} ConcurrentQueue;
void enqueue(ConcurrentQueue* queue, void* value);
void* dequeue(ConcurrentQueue* queue);