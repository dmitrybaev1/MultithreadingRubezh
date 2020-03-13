.PHONY: clean all
CFLAGS=-std=gnu99 -Wall -Werror -pedantic
Queue.o: Queue.c
	gcc $(CFLAGS) -c Queue.c
main.o: main.c
	gcc $(CFLAGS) -c main.c
all: clean Queue.o main.o
	gcc -o task main.o Queue.o
clean:
	rm -f *.o *.exe