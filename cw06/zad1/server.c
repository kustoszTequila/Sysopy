#include <unistd.h> 
#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#include "chat.h"
int queueGet = -1;
key_t serverKey = -1;
int isOnline = 1;
void removeQueue()
{

}
void parseMessage(struct message* msg)
{

}
int main (int arg, char** argv)
{
	// atexit
	// SIGINT
	struct msqid_ds queue;
	char* path = getenv("HOME");
	if (path == 0)
	{
		printf("Wrong path \n");
		exit(1);
	}
	serverKey = ftok(path,PROJECT_ID);
	if( serverKey == -1)
	{
		printf("ftok server error \n");
		exit(1);
	}
	queueGet = msgget(serverKey, IPC_CREAT | IPC_EXCL | 0666);
	if (queueGet == -1)
	{
		printf("msgget server error \n");
		exit(1);
	}
	printf("Działam sb\n");
	struct message msg;
	while (1)
	{
		printf("Działam sb\n");
		if ( isOnline != 1)
		{
			printf("Server quit\n");
			exit(1);
		}
		if (msgrcv(queueGet, &msg, sizeof (struct messageText), 0, 0) == -1)
		{
		 parseMessage(&msg);
		}   
	}
	return 0;
	
}
