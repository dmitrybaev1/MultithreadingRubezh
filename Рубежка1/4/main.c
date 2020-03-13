#include <stdlib.h>
#include <stdio.h>
#include "queue.h"
int main(){

    ConcurrentQueue queue = {.first = NULL, .mutex = PTHREAD_MUTEX_INITIALIZER};

    int firstValue = 2;
    int secondValues = 3;

    enqueue(&queue, &firstValue);
    enqueue(&queue, &secondValues);

    dequeue(&queue);
    dequeue(&queue);
	printf("done!");
	getchar();
}