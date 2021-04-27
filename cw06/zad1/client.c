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


int clientGet = -1;
key_t clientKey = -1;
int isOnline = 1;
int sesID = -1;
int queueGet = -1;
int chatM = 0;
int client2qid = -1;
pid_t client2pid = -1;

void removeQueue()
{
	if (msgctl(clientGet,IPC_RMID,NULL) == -1)
	{
		printf("Error while deleting client queue");
		exit(1);
	}
	else
	{
		printf("Successfully removed client queue");
	}
}
void disconnect()
{
	
	struct message* msg = calloc(1,sizeof(struct message*));
	msg->qid = clientGet;
	msg->mKey = clientKey;
	msg->mtype = DISCONNECT;
	strcpy(msg->messageT.buffor,"Discon");
	if (msgsnd(queueGet,msg,sizeof(struct message*),0) == -1)
	{
		printf("Error in disconnect command \n");
		exit(1);
	}
	if (chatM == 1)
	{
		chatM = 0;
		kill(client2pid,SIGUSR1);
		if ( msgsnd(client2qid, msg, sizeof(struct message), 0) == -1)
		{
		    	printf("Error in chat\n");
	    		exit(1);
		}
		client2qid = -1;
		client2pid = -1;
	}
	

}
void parseChat(char* command)
{
	if(strstr(command,"DISCONNECT") != NULL)
		return;
	struct message msg ;
	msg.qid = clientGet;
	msg.mKey = clientKey;
	msg.mtype = CHAT;
	strcpy(msg.messageT.buffor,command);
	kill(client2pid,SIGUSR1);
	if ( msgsnd(client2qid, &msg, sizeof(struct message), 0) == -1)
	{
		printf("Error in chat\n");
	    	exit(1);
	}
		

}
void usrHandler(int signum)
{
	struct message msg;	
	printf("TYP: %d\n",msg.mtype);
	if (chatM == 1)
	{
		if ( msgrcv(clientGet, &msg, sizeof(struct message), 0, 0) < 0)
	    	{
			printf("Server response error\n");
	    		exit(1);
		}
		if (msg.mtype == DISCONNECT)
		{
			printf("ENDING CHAT \n");
			chatM = 0;
			disconnect();
			return;
		}
		else
		{
			printf("%s\n",msg.messageT.buffor);
			fflush(stdout);
		}			
	}
	else if (chatM == 0)
	{
		if ( msgrcv(clientGet, &msg, sizeof(struct message), 0, 0) < 0)
	    	{
		    printf("Server response error\n");
	    		exit(1);
		}
		client2qid = msg.qid;
		client2pid = msg.sndPid;
		chatM = 1;	
		printf("CHAT START\n");
		return;
	
	}
	return;
	
}
void openChat()
{
	struct message msg;
	msg.mtype = INIT;
	msg.qid = clientGet;
	msg.mKey = clientKey;
	msg.sndPid = getpid();
	strcpy(msg.messageT.buffor,"Init");
	if ( msgsnd(queueGet, &msg, sizeof(struct message), 0) == -1)
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

    printf("Client registered with session ID: %d.\n", sesID);
}
void list()
{
	struct message* msg= calloc(1,sizeof(struct message*));;
	msg->qid = clientGet;
	msg->mKey = clientKey;
	msg->mtype = LIST;
	strcpy(msg->messageT.buffor,"List");
	if (msgsnd(queueGet,msg,sizeof(struct message*),0) == -1)
	{
		printf("Error in list command \n");
		exit(1);
	}
	struct message msgR;
	if ( msgrcv(clientGet, &msgR, sizeof(struct message), 0, 0) < 0)
    	{
	    	printf("Server response error\n");
    		exit(1);
	}
	printf("%s",msgR.messageT.buffor);
}
void connect(char * to)
{
	struct message msg;
	msg.qid = clientGet;
	msg.mKey = clientKey;
	msg.mtype = CONNECT;
	msg.sndPid = getpid();
	strcpy(msg.messageT.buffor,to);
	if (msgsnd(queueGet,&msg,sizeof(struct message),0) == -1)
	{
		printf("Error in command command \n");
		exit(1);
	}
	struct message msg2;
	if ( msgrcv(clientGet, &msg2, sizeof(struct message), 0, 0) < 0)
    	{
	    printf("Server response error\n");
    		exit(1);
	}
	if (msg2.mtype == DISCONNECT)
	{
		printf("COULDN'T CONNECT\n");
		return;
	}
	client2qid = msg2.qid; 
	client2pid = msg2.sndPid;
	chatM = 1;
	printf("CHAT START\n");
}
void stop()
{
	disconnect();
	struct message msg;
	msg.qid = clientGet;
	msg.mKey = clientKey;
	msg.mtype = STOP;
	strcpy(msg.messageT.buffor,"Stop");
	if (msgsnd(queueGet,&msg,sizeof(struct message),0) == -1)
	{
		printf("Error in stop command \n");
		exit(1);
	}
	exit(0);
}
void handler (int signum)
{
	printf("SIGINT received\n");
	stop();
}
int main (int argc, char **argv)
{
	if (signal(SIGINT,handler) == SIG_ERR)
	{
		printf("Error with signal handler\n");
		exit(1);
	}
	if (signal(SIGUSR1,usrHandler) == SIG_ERR)
	{
		printf("Error with USR1 handler\n");
		exit(1);
	}
	struct msqid_ds queue;
	char* path = getenv("HOME");
	if (path == 0)
	{
		printf("msgget client error \n");
		exit(1);
	}
	clientKey = ftok(path,CHAT_ID);
	if (clientKey == -1)
	{
		printf("Client ftok error\n");
		exit(1);
	}
	queueGet = msgget(clientKey,0);
	if (queueGet == -1)
	{
		printf("Client msgget error\n");
		exit(1);
	}
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
	if (atexit(removeQueue) != 0 )
	{
		printf("error while setting atexit\n");
		exit(1);
	}
	openChat();	
	char* buf[MSG_MAXS];	
        while (1) 
        {
        gets((char *) buf);
	if (strstr(buf,"DISCONNECT") != NULL)   
	{
		disconnect();
	}

        else if (chatM == 1) 
        {
            parseChat((char*)buf);
            continue;
        }

	else if (strstr(buf,"LIST") != NULL)
	{
		list();
	}
	else if (strstr(buf,"STOP") != NULL)
	{
		stop();
	}
	else if ((strstr(buf,"CONNECT") != NULL) && (strstr(buf,"DISCONNECT") == NULL) )
	 {
		char * command;
		char* arguments;
		command = strtok_r(buf," ",&arguments);
                connect(arguments);               
        }
        else
        {
        	printf("UNKNOWN COMMAND\n");
        }
    }
	return 0;
	
}
