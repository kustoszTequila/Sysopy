CC = gcc -Wall

run: 
	$(CC) -o sender sender.c
	$(CC) -o catcher catcher.c
kill:
	./catcher KILL
queue:
	./catcher SIGQUEUE
real:
	./catcher SIGRT	
clean:
	rm -f *.o
	rm -f catcher
	rm -f sender



