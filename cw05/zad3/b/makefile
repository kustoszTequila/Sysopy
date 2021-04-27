CC = gcc -Wall

compile:
	$(CC) -o consumer consumer.c
	$(CC) -o producer producer.c
	$(CC) -o test1 test1.c
	$(CC) -o test2 test2.c
	$(CC) -o test3 test3.c
test:
	./test1 10
testSec:
	./test2 5	
testTrd:
	./test3 5	
runCon: 
	./consumer PIPE file.txt 30
	
runProd1:
	./producer PIPE 1 pro1.txt 30

runProd2:
	./producer PIPE 2 pro2.txt 30

runProd3:
	./producer PIPE 4 pro3.txt 30

runProd4:
	./producer PIPE 3 pro4.txt 30
clean:
	rm -f *.o
	rm -f consumer
	rm -f producer
	rm -f PIPE
	rm -f test1
	rm -f test2
	rm -f test3
	rm -f file*
	rm -f test3


