CC = gcc -Wall

all: clean compile test 


compile:
	make clean
	$(CC) -o main main.c -lrt -lpthread
	$(CC) -o maker maker.c -lrt -lpthread
	$(CC) -o provider provider.c -lrt -lpthread
test:
	make compile
	./main 3 5
	make clean
	
test2: 
	make compile
	./main 6 7
	make clean			
clean:
	rm -f *.o
	rm -f *.so
	rm -f *.a
	rm -f provider
	rm -f maker
	rm -f main

