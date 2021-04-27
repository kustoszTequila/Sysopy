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
#include <mqueue.h>

#include "chat.h"

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
mqd_t clientGet = -1;
int isOnline = 1;
int sesID = -1;
mqd_t queueGet = -1;
int chatM = 0;
mqd_t client2 = -1;
pid_t client2pid = -1;
char path[512];
void disconnect()
{	
	struct message msg;
	msg.sndPid = getpid();
	strcpy(msg.buffor,"DISCONNECT");
	if (mq_send(queueGet,(char*) &msg,sizeof(struct message),DISCONNECT) == -1)
	{
		printf("Error in disconnect command \n");
		exit(1);
	}
	
	if (chatM == 1)
	{
		chatM = 0;
		kill(client2pid,SIGUSR1);
		if ( mq_send(client2, (char*) &msg, sizeof(struct message), DISCONNECT) == -1)
		{
		    	printf("Error in chat\n");
	    		exit(1);
		}
		mq_close(client2);
		client2 = -1;
		client2pid = -1;
	}
	chatM = 0;

}
void removeQueue()
{
	disconnect();
	struct message msg;
	msg.sndPid = getpid();
	mq_send(queueGet, (char*) &msg,sizeof(struct message),STOP);
	mq_close(clientGet);
	mq_unlink(path);
}
void parseChat(char* command)
{
	if(strstr(command,"DISCONNECT") != NULL)
		{
			disconnect();
		}
	struct message msg ;
	msg.sndPid = getpid();
	strcpy(msg.buffor,command);
	kill(client2pid,SIGUSR1);
	if ( mq_send(client2,(char*) &msg, sizeof(struct message), CONNECT) == -1)
	{
		printf("Error in chat\n");
	    	exit(1);
	}
		

}
void usrHandler(int signum)
{
	struct message msg;	
	if (chatM == 1)
	{
		if ( mq_receive(clientGet,(char*) &msg, sizeof(struct message),NULL) < 0)
	    	{
			printf("Server response error\n");
	    		exit(1);
		}
		if (strstr(msg.buffor,"DISCONNECT") != NULL)
		{
			printf("CHAT END\n");
			chatM = 0;
			disconnect();
		}
		else
		{
			printf("%s\n",msg.buffor);
			fflush(stdout);
		}			
	}
	else if (chatM == 0)
	{
		if ( mq_receive(clientGet,(char*) &msg, sizeof(struct message),NULL) < 0)
	    	{
		    printf("Server response error\n");
	    		exit(1);
		}
		client2pid = msg.sndPid;
		char  path2[512];
		sprintf((char*)path2,"/%d",client2pid);
		client2 = openQueue(path2);
		chatM = 1;	
		printf("CHAT START\n");
		return;
	
	}
	return;
	
}
void openChat()
{
	struct message msg;
	struct message rec;
	msg.sndPid = getpid();
	strcpy(msg.buffor,"Init");
	if ( mq_send(queueGet, (char*) &msg, sizeof(struct message), INIT) == -1)
	{
	    printf("Client init error\n");
    		exit(1);
	}
	if ( mq_receive(clientGet,(char*) &rec, sizeof(struct message),NULL) < 0)
    	{
	    printf("Server response error\n");
    		exit(1);
	}
	if (sscanf(rec.buffor, "%d", &sesID) < 1)
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
	struct message msg;
	msg.sndPid = getpid();
	strcpy(msg.buffor,"List");
	if (mq_send(queueGet,(char*) &msg,sizeof(struct message),LIST) == -1)
	{
		printf("Error in list command \n");
		exit(1);
	}
	struct message msgR;
	if ( mq_receive(clientGet, &msgR, sizeof(struct message), NULL) < 0)
    	{
	    	printf("Server response error\n");
    		exit(1);
	}
	printf("%s",msgR.buffor);
}
void connect(char * to)
{
	struct message msg;
	msg.sndPid = getpid();
	strcpy(msg.buffor,to);
	if (mq_send(queueGet,(char*) &msg,sizeof(struct message),CONNECT) == -1)
	{
		printf("Error in command command \n");
		exit(1);
	}
	struct message msg2;
	if ( mq_receive(clientGet,(char*) &msg2, sizeof(struct message),NULL) < 0)
    	{
	    printf("Server response error\n");
    		exit(1);
	}
	sscanf(msg2.buffor,"%d",&client2pid);
	if (client2pid == 0)
	{
		printf("COULDN'T CONNECT\n");
		return;
	}
	char  path2[512];
	sprintf((char*)path2,"/%d",client2pid);
	client2 = openQueue(path2);
	chatM = 1;	
	struct message msg3;
	msg3.sndPid = getpid();
	sprintf(msg.buffor,"%d",getpid());
	kill(client2pid,SIGUSR1);
	if (mq_send(client2,(char*) &msg3,sizeof(struct message),CONNECT) == -1)
	{
		printf("Error in command command \n");
		exit(1);
	}
	printf("CHAT START\n");
	struct sigevent notification;
	notification.sigev_notify = SIGEV_SIGNAL;
	notification.sigev_signo = SIGUSR1;
	mq_notify(clientGet, &notification);
	chatM = 1;
}
void stop()
{
	disconnect();
	struct message msg;
	msg.sndPid = getpid();
	strcpy(msg.buffor,"Stop");
	if (mq_send(queueGet,(char*) &msg,sizeof(struct message),STOP) == -1)
	{
		printf("Error in stop command \n");
		exit(1);
	}
	exit(0);
}
void handler (int signum)
{
	printf("SIGINT received\n");
	disconnect();
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
	if (atexit(removeQueue) != 0 )
	{
		printf("error while setting atexit\n");
		exit(1);
	}
	sprintf((char*) path, "/%d", getpid());
	queueGet = openQueue(SPATH);
	clientGet = createQueue(path);
	if (clientGet == -1)
	{
		printf("Can't make queue\n");
		exit(1);
	}
	openChat();	
	char* buf[MSG_MAXS];	
	struct sigevent notification;
	notification.sigev_notify = SIGEV_SIGNAL;
	notification.sigev_signo = SIGUSR1;
	mq_notify(clientGet, &notification);
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
