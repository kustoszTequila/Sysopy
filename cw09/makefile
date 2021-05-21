CC = gcc -Wall

run: compile test clean

compile:
	make clean
	$(CC) -o main main.c -lrt -lpthread

test:
	./main
			
clean:
	rm -f *.o
	rm -f main

