all: main

main: simcpu.o
	gcc -Wpedantic -std=gnu99 simcpu.c -o simcpu
clean:
	rm *.o
	rm simcpu
makeRun:
	make
	./simcpu
