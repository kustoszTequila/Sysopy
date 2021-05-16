CC = gcc -Wall

run: compile test clean

compile:
	make clean
	$(CC) -o main main.c -lrt -lpthread

test:
	./main 1 block apollonian_gasket.ascii.pgm out1.pgm
	./main 2 block apollonian_gasket.ascii.pgm out2.pgm
	./main 4 block apollonian_gasket.ascii.pgm out4.pgm
	./main 8 block apollonian_gasket.ascii.pgm out8.pgm
	./main 16 block apollonian_gasket.ascii.pgm out16.pgm
	./main 32 block apollonian_gasket.ascii.pgm out16.pgm
	./main 64 block apollonian_gasket.ascii.pgm out16.pgm
	./main 16 numbers apollonian_gasket.ascii.pgm outComparison.pgm
	./main 1 numbers casablanca.ascii.pgm outNum1.pgm
	./main 2 numbers casablanca.ascii.pgm outNum2.pgm
	./main 4 numbers casablanca.ascii.pgm outNum4.pgm
	./main 8 numbers casablanca.ascii.pgm outNum8.pgm
	./main 16 numbers casablanca.ascii.pgm outNum16.pgm
	./main 16 block casablanca.ascii.pgm outComparison2.pgm

			
clean:
	rm -f *.o
	rm -f *.so
	rm -f *.a
	rm -f main

