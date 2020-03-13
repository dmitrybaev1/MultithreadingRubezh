#include "myheader.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

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
    tp = pool_create(4);
    printf("after create\n");
    add_task(tp,worker,1);
    add_task(tp,worker,1);
    add_task(tp,worker,1);
    printf("after task\n");
    printf("before destroy\n");
    pool_destroy(tp);
    printf("after destroy\n");
    getch();
    return 0;
}