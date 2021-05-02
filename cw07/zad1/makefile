CC = gcc -Wall

all: clean compile 


compile:
	make clean
	$(CC) -o main main.c
	$(CC) -o maker maker.c -lrt
	$(CC) -o provider provider.c 
		
clean:
	rm -f *.o
	rm -f *.so
	rm -f *.a
	rm -f provider
	rm -f maker
	rm -f main

