#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char* argv[])
{
	int n = strtol(argv[1], NULL, 10);

	int* arr = malloc(sizeof(int) * n);

	srand(time(0));
	for (int i = 0; i < n; i++) {
		arr[i] = rand();
	}

	FILE* file = fopen("test", "w");
	
	fwrite(&n, sizeof(int), 1, file);
	fwrite(arr, sizeof(int), n, file);

	fclose(file);

	free(arr);

	return EXIT_SUCCESS;
}
