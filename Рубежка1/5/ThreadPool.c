#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "myheader.h"
#include <stddef.h>
/*Создать таск*/
Task* task_create(func_t func,void *arg)
{
    Task *task;
    if (func == NULL)
        return NULL;
    task = malloc(sizeof(*task));
    task->func = func;
    task->arg=arg;
    task->next = NULL;
    return task;
}
/*Уничтожить таск*/
void task_destroy(Task* task)
{
    if (task == NULL)
        return;
    free(task);
}
/*Получить таск*/
Task* task_get(ThreadPool *tp)
{
    Task *task;
    if (tp == NULL)
        return NULL;
    task = tp->task_first;
    if (task == NULL)
        return NULL;
    if (task->next == NULL) {
        tp->task_first = NULL;
        tp->task_last = NULL;
    } else {
        tp->task_first = task->next;
    }
    return task;
}
/*Вся работа пула здесь*/
void pool_worker(void *arg)
{
    ThreadPool *tp = arg;
    Task *task;
    while (1) {
        pthread_mutex_lock(&(tp->task_mutex));
        while (tp->task_first == NULL && !tp->stop) {
            pthread_cond_wait(&(tp->task_cond), &(tp->task_mutex));
        }
        if (tp->stop)
            break;
        task = task_get(tp);
        tp->working_cnt++;
        pthread_mutex_unlock(&(tp->task_mutex));
        if (task != NULL) {
            task->func(task->arg);
            task_destroy(task);
        }
        pthread_mutex_lock(&(tp->task_mutex));
        tp->working_cnt--;
        if (!tp->stop && tp->working_cnt == 0 && tp->task_first == NULL)
            pthread_cond_signal(&(tp->working_cond));
        pthread_mutex_unlock(&(tp->task_mutex));
    }
    tp->thread_cnt--;
    pthread_cond_signal(&(tp->working_cond));
    pthread_mutex_unlock(&(tp->task_mutex));
}
/*Создать пул*/
ThreadPool* pool_create(int num)
{
    ThreadPool *tp;
    pthread_t thread;
    int i;
    if (num <= 0)
        num = 2;
    tp = calloc(1, sizeof(*tp));
    tp->thread_cnt = num;
    pthread_mutex_init(&(tp->task_mutex), NULL);
    pthread_cond_init(&(tp->task_cond), NULL);
    pthread_cond_init(&(tp->working_cond), NULL);
    tp->task_first = NULL;
    tp->task_last  = NULL;
    for (i=0; i<num; i++) {
        pthread_create(&thread, NULL, pool_worker, tp);
        pthread_detach(thread);
    }
    return tp;
}

/*Уничтожить пул*/
void pool_destroy(ThreadPool *tp)
{
    Task *task;
    Task *task2;
    if (tp == NULL)
        return;
    pthread_mutex_lock(&(tp->task_mutex));
    task = tp->task_first;
    while (task != NULL) {
        task2 = task->next;
        task_destroy(task);
        task = task2;
    }
    tp->stop = 1;
    pthread_cond_broadcast(&(tp->task_cond));
    pthread_mutex_unlock(&(tp->task_mutex));
    pool_wait(tp);
    pthread_mutex_destroy(&(tp->task_mutex));
    pthread_cond_destroy(&(tp->task_cond));
    pthread_cond_destroy(&(tp->working_cond));
    free(tp);
}
/*Накинуть задачу в пул*/
void add_task(ThreadPool *tp, func_t func,void *arg)
{
    Task *task;
    task = task_create(func,arg);
    pthread_mutex_lock(&(tp->task_mutex));
    if (tp->task_first == NULL) {
        tp->task_first = task;
        tp->task_last = tp->task_first;
    } else {
        tp->task_last->next = task;
        tp->task_last = task;
    }
    pthread_cond_broadcast(&(tp->task_cond));
    pthread_mutex_unlock(&(tp->task_mutex));
}
/*
 * Это блокирующая функция, которая будет возвращаться только тогда, когда нет работы. Мьютекс заблокирован, и мы ожидаем
 * условно, если есть какие-либо потоки, обрабатывающие, если еще есть работа, или если потоки останавливаются и не все вышли.
 * Повторная попытка является мерой безопасности в случае ложного пробуждения. После обработки ничего не возвращается,
 * чтобы вызывающий мог продолжить.
 * */
void pool_wait(ThreadPool *tp)
{
    if (tp == NULL)
        return;
    pthread_mutex_lock(&(tp->task_mutex));
    while (1) {
        if ((!tp->stop && tp->working_cnt != 0) || (tp->stop && tp->thread_cnt != 0)) {
            pthread_cond_wait(&(tp->working_cond), &(tp->task_mutex));
        } else {
            break;
        }
    }
    pthread_mutex_unlock(&(tp->task_mutex));
}

