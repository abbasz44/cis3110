all: main

main: FSreport.o 
	gcc -Wpedantic -std=gnu99 FSreport.c -o FSreport

	
clean:
	rm *.o
	rm FSreport
makeRun:
	make
	./FSreport