CC = gcc -Wall

all: clean compile 


compile:
	make clean
	$(CC) -o server server.c 
	$(CC) -o client client.c 
		
clean:
	rm -f *.o
	rm -f *.so
	rm -f *.a
	rm -f server
	rm -f client

