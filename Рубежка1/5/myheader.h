#include <pthread.h>


typedef void (*func_t)(void *arg);

typedef struct Task {
    func_t func;
    void *arg;
    struct Task* next;
}Task;

typedef struct QueueNode {
    Task* value;
    struct QueueNode* next;
} QueueNode;

typedef struct ConcurrentQueue{
    QueueNode* first;
    pthread_mutex_t mutex;
} ConcurrentQueue;

typedef struct ThreadPool{
    Task* task_first;
    Task* task_last;
    pthread_mutex_t  task_mutex;
    pthread_cond_t task_cond;//сигнализирует потокам, что есть работа
    pthread_cond_t working_cond;//сигнализирует, когда нет обработки потоков
    int working_cnt;//сколько потоков активно
    int thread_cnt;//кол-во потоков
    int stop;//останов
}ThreadPool;

Task* task_create(func_t func, void *arg);
void task_destroy(Task* task);
Task* task_get(ThreadPool *tp);
void pool_worker(void *arg);
ThreadPool* pool_create(int num);
void pool_destroy(ThreadPool *tp);
void add_task(ThreadPool *tp, func_t func,void *arg);
void pool_wait(ThreadPool *tp);

void enqueue(ConcurrentQueue* queue, Task* value);
Task* dequeue(ConcurrentQueue* queue);
