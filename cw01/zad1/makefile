CC = gcc -Wall

all: clean static shared


static:
	make clean
	$(CC) -c bloclib.c 
	ar crs libbloclib.a bloclib.o
	
shared:
	make clean
	$(CC) -c -fPIC bloclib.c
	$(CC) -shared -fPIC -o libbloclib.so bloclib.o
	
clean:
	rm -f *.o
	rm -f *.so
	rm -f *.a

