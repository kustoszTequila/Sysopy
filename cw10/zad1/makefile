CC = gcc -Wall

run: compile test clean

compile:
	make clean
	$(CC) -o server server.c -lrt -lpthread
	$(CC) -o client client.c 
	
server:
	./server 7001 socket
		
user:
	./user stary local socket		
			
clean:
	rm -f *.o
	rm -f server
	rm -f client
	rm -f socket 

