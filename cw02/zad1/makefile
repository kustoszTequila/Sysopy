CC = gcc -Wall
all: lib sys

lib: 
	$(CC) main.lib.c -o main  
	chmod +x main
	./main a.txt b.txt 
	
sys:
	$(CC) main.sys.c -o main  
	chmod +x main
	./main a.txt b.txt 
