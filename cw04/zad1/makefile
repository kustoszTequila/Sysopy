CC = gcc -Wall

run: 
	$(CC) -o main main.c
	$(CC) -o mainExec mainExec.c
	$(CC) -o ex ex.c
	./main ignore
	./main handler
	./main pending
	./main mask mask
	./mainExec ignore
	./mainExec mask
	./mainExec pending
	make clean
	
clean:
	rm -f *.o
	rm -f main
	rm -f ex
	rm -f mainExec


