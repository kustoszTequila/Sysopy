#include <unistd.h> 
#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <mqueue.h>

#include "chat.h"
mqd_t queue;
int isOnline = 1;
mqd_t clientsQueues[CLIENTS_MAX];
int areAvailable[CLIENTS_MAX];
pid_t pids[CLIENTS_MAX];
int indeks= 0;

mqd_t createQueue(const char* path)
{
	struct mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = sizeof(struct message); 
	
	mqd_t id = mq_open(path, O_RDONLY | O_EXCL | O_CREAT, 0660, &attr);
	if (id == -1)
	{
		printf("ERROR while creating a queue\n");
		exit(1);
	}
	
	return id;

}
mqd_t openQueue(const char* path)
{
	mqd_t id = mq_open(path, O_WRONLY);
	if (id == -1)
	{
		printf("can't open server queue\n");
		exit(1);
	}
	return id;
}
void fillArrays()
{
	for (int i =0; i<CLIENTS_MAX;i++)
	{
		clientsQueues[i] = -1;
		areAvailable[i] = 0;
		pids[i] = -1;
	}
}
void removeQueue()
{
	for (int i = 0; i < indeks; i++)
	{
		mq_close(clientsQueues[i]);
		kill(pids[i],SIGINT);
	}
	mq_unlink(SPATH);
}
void handler(int signum)
{
	printf("SIGINT received (server) \n");
	isOnline = 0;
	exit(0);
}
void init(struct message* msg)
{
	struct message mess;
	pids[indeks] = msg->sndPid;
	areAvailable[indeks] = 1;
	if (indeks >= CLIENTS_MAX)
	{
		printf("Already max number of clients\n");
		strcpy(msg->buffor,"-1");
	}
	char path[25];
    	sprintf((char*)path, "/%d", msg->sndPid);
    	mqd_t id = openQueue(path);
    	if(id == -1)
    	{
		printf("Error while opening queue\n");
		exit(1);
    	}
	printf("Sending ID: %d\n",indeks);
	sprintf(mess.buffor,"%d",indeks);			
	clientsQueues[indeks] = id;
	mess.sndPid = getpid();
	indeks++;
	if(mq_send(clientsQueues[indeks-1],(char*) &mess, sizeof(struct message),INIT) == -1)
	{
		printf ("Initializing chat filed\n");
		exit(1);
	}
	while ( indeks < CLIENTS_MAX && clientsQueues[indeks] > -1)
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
		if(pids[i] != -1)			
		{
			printf(": %d ", i);
			sprintf(avail + strlen(avail)," %d ",i);
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
	struct message msg2;
	printf("%s\n",avail);
	strcpy(msg2.buffor,avail);
	int to;
	for (int i =0 ; i <CLIENTS_MAX; i++)
	{
		if (pids[i] == msg->sndPid)
		{
			to= i;			
			break;
		}
	}
	msg2.sndPid = getpid();
	if(mq_send(clientsQueues[to],(char*)  &msg2, sizeof(struct message),LIST) == -1)
	{
		printf ("LIST ERROR\n");
		exit(1);
	}

	fflush(stdin);
	fflush(stdout);
	free(avail);
}
void stop(struct message* msg)
{


	int toDelete = msg->sndPid;
	for(int i = 0; i < CLIENTS_MAX; i++)
	{
		if (pids[i] == toDelete)
		{
			clientsQueues[i] = -1;
			areAvailable[i] = 0;
			pids[i] = -1;
			indeks = i;
			break;
		}
	}
	printf("Client number: %d deleted\n", toDelete);
}
void connect(struct message* msg)
{
	int client = msg->sndPid;
	int clinetInd = -1;
	for (int i =0 ; i <CLIENTS_MAX; i++)
	{
		if (pids[i] == client)
		{
			clinetInd= i;			
			break;
		}
	}
	int toConnect = atoi(msg->buffor);
	struct message mess;
	mess.sndPid = getpid();
	sprintf(mess.buffor,"%d",pids[toConnect]);
	if (areAvailable[toConnect] == 0)
	{
		printf("klient jest zajęty\n");
		return;
	}
	if(mq_send(clientsQueues[clinetInd],(char*)  &mess, sizeof(struct message),CHAT) == -1)
	{
		printf ("Sending qid to client 1 error\n");
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
		if (pids[i] == msg->sndPid)
		{
			printf("PID Array: %d ",pids[i]);
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
void parseMessage(struct message* msg,int type)
{
	switch(type)
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
		default:
			printf("error while reading command\n");
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
	queue = createQueue(SPATH);
	if (atexit(removeQueue) != 0 )
	{
		printf("error while setting atexit\n");
		exit(1);
	}
	struct message msg;
	uint type;
	printf("SERVER START\n");
	while (1)
	{
		if ( isOnline != 1)
		{
			printf("Server quit\n");
			exit(1);
		}
		if (mq_receive(queue,(char*) &msg, sizeof (struct message),&type) == -1)
		{
			printf("Server error while reading message \n");
			exit(1);
		}   
		 parseMessage(&msg,type);
		 fflush(stdin);
	}
	return 0;
	
}
