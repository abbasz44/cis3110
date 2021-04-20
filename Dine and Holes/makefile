all: main

main: dine.o
	gcc -Wpedantic -std=gnu99 dine.c -o dine -lpthread
	gcc -Wpedantic -std=gnu99 holes.c -o holes

clean:
	rm *.o
	rm dine
	rm holes
