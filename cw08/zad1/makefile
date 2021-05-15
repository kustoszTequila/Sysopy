CC = gcc -Wall


compile:
	make clean
	$(CC) -o main main.c -lrt -lpthread

test:
	./main 30 block apollonian_gasket.ascii.pgm out.pgm
test2:
	./main 15 numbers apollonian_gasket.ascii.pgm out.pgm
			
clean:
	rm -f *.o
	rm -f *.so
	rm -f *.a
	rm -f main

