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
#include <signal.h>

#include "chat.h"
int queueGet = -1;
key_t serverKey = -1;
int isOnline = 1;
int clientsQid[CLIENTS_MAX];
key_t clientsKeys[CLIENTS_MAX];
int areAvailable[CLIENTS_MAX];
pid_t pids[CLIENTS_MAX];
int indeks= 0;
void fillArrays()
{
	for (int i =0; i<CLIENTS_MAX;i++)
	{
		clientsQid[i] = -1;
		areAvailable[i] = 0;
		pids[i] = -1;
	}
}
void removeQueue()
{
	if (msgctl(queueGet,IPC_RMID,NULL) == -1)
	{
		printf("Error while deleting queue");
		exit(1);
	}
	else
	{
		printf("Successfully removed queue");
	}
}
void handler(int signum)
{
	printf("SIGINT received (server) \n");
	isOnline = 0;
	exit(0);
}
void init(struct message* msg)
{
	clientsQid[indeks] = msg->qid;
	clientsKeys[indeks] = msg->mKey;
	pids[indeks] = msg->sndPid;
	areAvailable[indeks] = 1;
	if (indeks >= CLIENTS_MAX)
	{
		printf("Already max number of clients\n");
		strcpy(msg->messageT.buffor,"-1");
	}
	else
	{
		printf("Sending ID: %d\n",indeks);
		sprintf(msg->messageT.buffor,"%d",indeks);
		indeks++;
	}
	
	if(msgsnd(clientsQid[indeks-1], msg, sizeof(struct message),0) == -1)
	{
		printf ("Initializing chat filed\n");
		exit(1);
	}
	while ( indeks < CLIENTS_MAX && clientsQid[indeks] > -1)
	{
		indeks++;
	}
}
void list(struct message* msg)
{
	char *avail = calloc(1024,sizeof(char));
	printf("Connected clients: \n");
	for (int i = 0; i < CLIENTS_MAX; i++)
	{
		if(clientsQid[i] != -1)			
		{
			printf(": %d ", clientsQid[i]);
			sprintf(avail + strlen(avail)," %d ",clientsQid[i]);
			if(areAvailable[i] == 1)
			{
				printf("available\n");
				sprintf(avail + strlen(avail),"available\n");
			}
				
			else
			{
				printf("unavailable\n");
				sprintf(avail+ strlen(avail),"unavailable\n");			
			}

		}
	}	
	printf("%s\n",avail);
	int to = msg->qid;
	strcpy(msg->messageT.buffor,avail);
	if(msgsnd(to, msg, sizeof(struct message),0) == -1)
	{
		printf ("Initializing chat filed\n");
		exit(1);
	}
	fflush(stdin);
	fflush(stdout);
	free(avail);
}
void stop(struct message* msg)
{


	int toDelete = msg->qid;
	for(int i = 0; i < CLIENTS_MAX; i++)
	{
		if (clientsQid[i] == toDelete)
		{
			clientsQid[i] = -1;
			areAvailable[i] = 0;
			pids[i] = -1;
			indeks = i;
		}
	}
	printf("Client number: %d deleted\n", toDelete);
}
void connect(struct message* msg)
{
	int client = msg->qid;
	int clinetInd = -1;
	for (int i =0 ; i <CLIENTS_MAX; i++)
	{
		if (clientsQid[i] == client)
		{
			clinetInd= i;			
			break;
		}
	}
	int toConnect = atoi(msg->messageT.buffor);
	if (toConnect == clinetInd)
	{
	
		printf("NIE MOŻNA SIĘ POŁĄCZYĆ Z SAMYM SOBĄ\n");
		msg->mtype = DISCONNECT;
		if(msgsnd(client, msg, sizeof(struct message),0) == -1)
		{
			printf ("Sending to client 1 error\n");
			exit(1);
		}
		return;
	}
	if (areAvailable[clinetInd] == 0)
	{
		printf("klient jest zajęty\n");
		return;
	}
	pid_t pidd = pids[clinetInd];
	msg->mtype = CHAT;
	msg->qid = clientsQid[toConnect];
	msg->sndPid = pids[toConnect];
	if (areAvailable[toConnect] == 0)
	{
		printf("klient jest zajęty\n");
		return;
	}
	if(msgsnd(client, msg, sizeof(struct message),0) == -1)
	{
		printf ("Sending qid to client 1 error\n");
		exit(1);
	}
	msg->qid = client;
	msg->mtype = CHAT;
	msg->sndPid = pidd;
	kill(pids[toConnect],SIGUSR1);
	if(msgsnd(clientsQid[toConnect], msg, sizeof(struct message),0) == -1)
	{
		printf ("Sending qid to client 2 error\n");
		exit(1);
	}
	areAvailable[clinetInd] = 0;
	areAvailable[toConnect] = 0;
}
void disconnect(struct message* msg)
{
	int ID = -1;
	for (int i =0 ; i <CLIENTS_MAX; i++)
	{
		if (clientsQid[i] == msg->qid)
		{
			printf("QID Array: %d ",clientsQid[i]);
			ID= i;
			break;
		}
	}
	if (ID == -1)
	{
		printf("can't delete - qid doesn't exist");
		return;
	}
	areAvailable[ID] = 1;
}
void parseMessage(struct message* msg)
{
	switch(msg->mtype)
	{

		case STOP:
			stop(msg);	
		break;
		case DISCONNECT:
			disconnect(msg);		
		break;
		case LIST:
			list(msg);
		break;
		case CONNECT:
			connect(msg);
		break;
		case INIT:
			init(msg);	
		break;		
	}
}
int main (int arg, char** argv)
{
	fillArrays();
	if (signal(SIGINT,handler) == SIG_ERR)
	{
		printf("Error with signal handler\n");
		exit(1);
	}	
	struct msqid_ds queue;
	char* path = getenv("HOME");
	if (path == 0)
	{
		printf("Wrong path \n");
		exit(1);
	}
	serverKey = ftok(path,CHAT_ID);
	if( serverKey == -1)
	{
		printf("ftok server error \n");
		exit(1);
	}
	queueGet = msgget(serverKey, IPC_CREAT  | 0666);
	if (queueGet == -1)
	{
		printf("msgget server error \n");
		exit(1);
	}
	if (atexit(removeQueue) != 0 )
	{
		printf("error while setting atexit\n");
		exit(1);
	}
	printf("SERVER KEY: %ld, queueGet: %d \n",serverKey,queueGet);
	struct message msg;
	while (1)
	{
		if ( isOnline != 1)
		{
			printf("Server quit\n");
			exit(1);
		}
		if (msgrcv(queueGet, &msg, sizeof (struct message), 0, 0) == -1)
		{
			printf("Server error while reading message \n");
			exit(1);
		}   
		 parseMessage(&msg);
		 fflush(stdin);
	}
	return 0;
	
}
