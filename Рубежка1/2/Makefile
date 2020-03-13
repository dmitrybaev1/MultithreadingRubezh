.PHONY: clean all
CFLAGS=-std=gnu99 -Wall -Werror -pedantic
ThinList.o: ThinList.c
	gcc $(CFLAGS) -c ThinList.c
main.o: main.c
	gcc $(CFLAGS) -c main.c
all: clean ThinList.o main.o
	gcc -o task main.o ThinList.o
clean:
	rm -f *.o *.exe