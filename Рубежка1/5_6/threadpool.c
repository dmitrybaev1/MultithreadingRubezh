#include "threadpool.h"

#include <stdlib.h>

void* worker_job(void* args)
{
	ThreadPool* tp = args;
    while(1) {
		pthread_testcancel();
		ThreadPoolTask* task = dequeue(tp->task_queue);
		if (task != NULL) {
			(*task->job)(task->data);
			free(task);
		} else {
			pthread_mutex_lock(&tp->task_mutex);
			tp->free_workers += 1;
			pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
			pthread_cond_wait(&tp->task_cond, &tp->task_mutex);
			pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
			pthread_mutex_unlock(&tp->task_mutex);
		}
	}
}

ThreadPool* threadpool_create(int worker_count)
{
	ThreadPool* tp = malloc(sizeof(ThreadPool));
	tp->worker_count = worker_count;
	pthread_mutex_init(&tp->task_mutex, NULL);
	pthread_cond_init(&tp->task_cond, NULL);
	tp->free_workers = 0;
	tp->task_queue = queue_create();
	tp->worker = malloc(sizeof(pthread_t) * worker_count);
	for (int i = 0; i < worker_count; i++) {
		pthread_create(&tp->worker[i], NULL, &worker_job, tp);
	}
	return tp;
}

void threadpool_add_task(ThreadPool* tp, void (*job)(void*), void* data)
{
	ThreadPoolTask* task = malloc(sizeof(ThreadPoolTask));
	task->job = job;
	task->data = data;
	enqueue(tp->task_queue, task);
	pthread_mutex_lock(&tp->task_mutex);
	if (tp->free_workers > 0) {
		tp->free_workers -= 1;
		pthread_cond_signal(&tp->task_cond);
	}
	pthread_mutex_unlock(&tp->task_mutex);
}

void threadpool_destroy(ThreadPool* tp)
{
	for (int i = 0; i < tp->worker_count; i++) {
		pthread_cancel(tp->worker[i]);
	}
	pthread_mutex_lock(&tp->task_mutex);
	pthread_cond_broadcast(&tp->task_cond);
	pthread_mutex_unlock(&tp->task_mutex);
	for (int i = 0; i < tp->worker_count; i++) {
		pthread_join(tp->worker[i], NULL);
	}
	free(tp->worker);
	pthread_mutex_destroy(&tp->task_mutex);
	pthread_cond_destroy(&tp->task_cond);
	queue_destroy(tp->task_queue);
	free(tp);
}
