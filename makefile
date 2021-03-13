CC = gcc -Wall

static:
	make clean
	$(CC) -c bloclib.c 
	ar rcs bloclib.a bloclib.o
	
shared:
	make clean
	$(CC) -fPIC –c bloclib.c
	$(CC) -shared -fPIC -o bloclib.so bloclib.o
	
clean:
	rm -f *.o
	
