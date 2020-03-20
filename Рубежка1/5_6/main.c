#include "threadpool.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#define W_COUNT 3
#define PART_COUNT 6

typedef struct SortData {
	int* arr;
	int from, to;
	int part_count;
	int* ready_count;
	pthread_mutex_t* ready_mutex;
	pthread_cond_t* ready_cond;
} SortData;

int comp(const void* a_ptr, const void* b_ptr)
{
	int a = *((int*) a_ptr);
	int b = *((int*) b_ptr);
	return a < b ? -1 : (a > b ? 1 : 0);
}

int verify_sorted(int* arr, int n)
{
    printf("%d\n",arr[0]);
	for (int i = 1; i < n; i++) {
		if (arr[i - 1] > arr[i]) {
		    printf("%d error!\n",arr[i]);
			return 0;
		}
        printf("%d\n",arr[i]);
	}
	return 1;
}

void sort_job(void* args)
{
	SortData* data = args;
	int* ptr = &data->arr[data->from];
	int count = data->to - data->from;
	qsort(ptr, count, sizeof(int), &comp);
	pthread_mutex_lock(data->ready_mutex);
	*data->ready_count += 1;
	if (*data->ready_count == data->part_count) {
		pthread_cond_signal(data->ready_cond);
	}
	pthread_mutex_unlock(data->ready_mutex);
	free(data);
}

int main()
{
	FILE* file = fopen("test", "r");
	int n;
	fread(&n, sizeof(int), 1, file);
	int* arr = malloc(sizeof(int) * n);
	fread(arr, sizeof(int), n, file);
	fclose(file);
	ThreadPool* tp = threadpool_create(W_COUNT);
	int ready_count = 0;
	pthread_mutex_t ready_mutex;
	pthread_mutex_init(&ready_mutex, NULL);
	pthread_cond_t ready_cond;
	pthread_cond_init(&ready_cond, NULL);
	int part_length = n / PART_COUNT;
	int part_limit[PART_COUNT];
	for (int i = 0; i < PART_COUNT - 1; i++) {
		part_limit[i] = (i + 1) * part_length;
	}
	part_limit[PART_COUNT - 1] = n;
	for (int i = 0; i < PART_COUNT; i++) {
		SortData* data = malloc(sizeof(SortData));
		data->arr = arr;
		data->from = i * part_length;
		data->to = part_limit[i];
		data->part_count = PART_COUNT;
		data->ready_count = &ready_count;
		data->ready_mutex = &ready_mutex;
		data->ready_cond = &ready_cond;
		threadpool_add_task(tp, &sort_job, data);
	}
	int* sorted_arr = malloc(sizeof(int) * n);
	int part_pos[PART_COUNT];
	for (int i = 0; i < PART_COUNT; i++) {
		part_pos[i] = i * part_length;
	}
	pthread_mutex_lock(&ready_mutex);
	if (ready_count < W_COUNT) {
		pthread_cond_wait(&ready_cond, &ready_mutex);
	}
	pthread_mutex_unlock(&ready_mutex);
	// Мердж
	for (int i = 0; i < n; i++) {
		int min_element = INT_MAX;
		int part_with_min;
		for (int j = 0; j < PART_COUNT; j++) {
			int pos = part_pos[j];
			if (pos == part_limit[j]) {
				continue;
			}
			if (arr[pos] < min_element) {
				min_element = arr[pos];
				part_with_min = j;
			}
		}
		sorted_arr[i] = min_element;
		part_pos[part_with_min] += 1;
	}
	printf("is_sorted=%d\n", verify_sorted(sorted_arr, n));
	free(arr);
	free(sorted_arr);
	pthread_mutex_destroy(&ready_mutex);
	pthread_cond_destroy(&ready_cond);
	threadpool_destroy(tp);
	return 0;
}
