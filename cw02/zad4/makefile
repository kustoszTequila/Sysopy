CC = gcc -Wall
all: lib sys

lib: 
	$(CC) main.lib.c -o main  
	chmod +x main
	./main dane.txt dane.txt out.txt in ZMIANA
	
sys:
	$(CC) main.sys.c -o main  
	chmod +x main
	./main dane.txt out.txt in ZMIANA
