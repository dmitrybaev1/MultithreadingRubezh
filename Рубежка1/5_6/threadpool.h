#include "queue.h"

typedef struct ThreadPoolTask {
	void (*job)(void*);
	void* data;
} ThreadPoolTask;

typedef struct ThreadPool {
	pthread_t* worker;
	int worker_count;
	Queue* task_queue;
	pthread_mutex_t task_mutex;
	pthread_cond_t task_cond;
	int free_workers;
} ThreadPool;

ThreadPool* threadpool_create(int);
void threadpool_destroy(ThreadPool*);

void threadpool_add_task(ThreadPool*, void (*)(void*), void*);
