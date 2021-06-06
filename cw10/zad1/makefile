CC = gcc -Wall

run: compile test clean

compile:
	make clean
	$(CC) -o server server.c -lrt -lpthread
	$(CC) -o client client.c 
	
test:
	make compile
	./server 7100 socket
		
user:
	./client uzer local socket
user2:
	./client uzer2 local socket
				
user3:
	./client uzer3 network 7100			
clean:
	rm -f *.o
	rm -f server
	rm -f client
	rm -f socket 

