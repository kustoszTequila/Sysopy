CC = gcc -Wall

compile:
	$(CC) -o consumer consumer.c
	$(CC) -o producer producer.c
	mkfifo PIPE
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


