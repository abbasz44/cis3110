all: main

main: myShell.o
	gcc -Wpedantic -std=gnu99 myShell.c -o myShell
clean:
	rm *.o
	rm myShell
makeRun:
	make
	./myShell