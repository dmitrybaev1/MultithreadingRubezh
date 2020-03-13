FLAGS=-Wall -Wextra --std=gnu99 -pedantic -Werror

all: merge

merge: main.o queue.o threadpool.o
	gcc -pthread -o $@ $^

test: test.o threadpool.o queue.o
	gcc -pthread -o $@ $^

gen: gen.c
	gcc -o $@ $^

test.o: test.c threadpool.h
	gcc -c $(FLAGS) test.c

main.o: main.c threadpool.h
	gcc -c $(FLAGS) main.c

queue.o: queue.c queue.h
	gcc -c $(FLAGS) queue.c

threadpool.o: threadpool.c threadpool.h queue.h
	gcc -c $(FLAGS) threadpool.c

clean:
	rm -f *.o merge test gen
