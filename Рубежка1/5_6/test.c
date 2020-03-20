#include "threadpool.h"

#include <stdio.h>
#include <unistd.h>

int main()
{
	ThreadPool* tp = threadpool_create(4);
	puts("Created");

	sleep(2);

	puts("Destroying...");
	threadpool_destroy(tp);

	return 0;
}
