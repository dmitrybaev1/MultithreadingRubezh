.PHONY: clean all
CFLAGS=-std=gnu99 -Wall -Werror -pedantic
RoughList.o: RoughList.c
	gcc $(CFLAGS) -c RoughList.c
main.o: main.c
	gcc $(CFLAGS) -c main.c
all: clean RoughList.o main.o
	gcc -o task main.o RoughList.o
clean:
	rm -f *.o *.exe