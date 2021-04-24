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

int clientGet = -1;
key_t clientKey = -1;
int isOnline = 1;
int sesID = -1;
void openChat()
{
	struct message msg;
	msg.mType = INIT;
	msg.qid = clientGet;
	msg.mKey = clientKey;
    
	strcpy(msg.messageT.buffor,"INIT\n");

	if ( msgsnd(clientGet, &msg, sizeof(struct message), 0) == -1)
	{
	    printf("Client init error\n");
    		exit(1);
	}
	if ( msgrcv(clientGet, &msg, sizeof(struct message), 0, 0) < 0)
    	{
	    printf("Server response error\n");
    		exit(1);
	}
	if (sscanf(msg.messageT.buffor, "%d", &sesID) < 1)
	{
	    printf("Sscanf error\n");
    		exit(1);
	}
	if (sesID < 0)
	{
		printf("Server is full\n");
		exit(1);
	}

    printf("Client registered with session ID %d.\n", sesID);
}
int main (int argc, char **argv)
{
	// atexit
	// SIGINT
	struct msqid_ds queue;
	char* path = getenv("HOME");
	if (path == 0)
	{
		printf("msgget client error \n");
		exit(1);
	}
	// kolejka
	clientKey = ftok(path,getpid());
	if( clientKey == -1)
	{
		printf("ftok client error \n");
		exit(1);
	}

	clientGet = msgget(clientKey, IPC_CREAT | IPC_EXCL | 0666);
	if (clientGet == -1)
	{
		printf("msgget client error \n");
		exit(1);
	}
	openChat();
	
	printf("Zamykam temat, następnym razem użyj opcji szukaj\n");
	return 0;
	
}
