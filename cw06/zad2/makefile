CC = gcc -Wall

all: clean compile 


compile:
	make clean
	$(CC) -o server server.c -lrt
	$(CC) -o client client.c -lrt
		
clean:
	rm -f *.o
	rm -f *.so
	rm -f *.a
	rm -f server
	rm -f client

