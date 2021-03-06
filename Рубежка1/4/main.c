#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "tpool.h"
static const size_t num_threads = 4;
static const size_t num_items   = 100;

void worker(void *arg)
{
    printf("hello!");
}

int main()
{
    ThreadPool *tp;
    int *vals;
    int i;
    printf("before\n");
    tp = pool_create(num_threads);
    printf("after create\n");
    add_task(tp,worker,1);
    printf("after task\n");
   /* vals = calloc(num_items, sizeof(*vals));
    for (i=0; i<num_items; i++) {
        vals[i] = i;
        add_task(tp, worker,vals+i);
    }
    pool_wait(tp);
    for (i=0; i<num_items; i++) {
        printf("%d\n", vals[i]);
    }
    free(vals);*/
    printf("before destroy\n");
    pool_destroy(tp);
    printf("after destroy\n");
    return 0;
}