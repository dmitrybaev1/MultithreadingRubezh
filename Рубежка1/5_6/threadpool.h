#include "queue.h"

typedef struct pthread_tPoolTask {
	void (*job)(void*);
	void* data;
} pthread_tPoolTask;

typedef struct pthread_tPool {
	pthread_t* worker;
	int worker_count;
	Queue* task_queue;
	pthread_mutex_t task_mutex;
	pthread_cond_t task_cond;
	int free_workers;
} pthread_tPool;

pthread_tPool* threadpool_create(int);
void threadpool_destroy(pthread_tPool*);

void threadpool_add_task(pthread_tPool*, void (*)(void*), void*);
