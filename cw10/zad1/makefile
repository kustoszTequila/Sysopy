CC = gcc -Wall

run: compile test clean

compile:
	make clean
	$(CC) -o server server.c -lrt -lpthread
	$(CC) -o client client.c 
			
clean:
	rm -f *.o
	rm -f server
	rm -f client

