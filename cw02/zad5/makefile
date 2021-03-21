CC = gcc -Wall
all: lib sys

lib: 
	$(CC) main.lib.c -o main -lm 
	chmod +x main
	./main dane.txt out.txt
	
sys:
	$(CC) main.sys.c -o main -lm 
	chmod +x main
	./main dane.txt out.txt
