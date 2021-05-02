#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h> 
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

#include "pizza.h"

int semId;
int memId;

union semun
{
	int val;
	struct semid_ds* buf;
	unsigned short * array;
	struct seminfo * _buf;
};
void makeSem()
{
	key_t key = ftok(getenv("HOME"),'A');
	semId = semget(key,(SEMNUM),IPC_CREAT | 0666);
	if (semId < 0 )
	{
		printf("ERROR while creating semaphore\n");
		exit(1);
	}

	union semun arg;
	arg.val = 0;
	for (int i = 0; i < SEMNUM;i++)
	{
		semctl(semId,i,SETVAL,arg);
	}
	arg.val = 5;
	semctl(semId,1,SETVAL,arg);
	semctl(semId,2,SETVAL,arg);

}
void makeMem()
{
	key_t key = ftok(getenv("HOME"),'B');
	memId = shmget(key,sizeof(struct pizza),IPC_CREAT | 0666);
	if (memId < 0)
	{
		printf("Error while creating memory\n");
		exit(1);		
	}
}
int main (int argc, char **argv)
{
	int N,M;
	if (argc != 3)
	{
		printf("Zła liczba argumentów\n");
		exit(1);	
	}
	else
	{
		N = atoi(argv[1]);
		M = atoi(argv[2]);
	}
	if (N <= 0 || M <= 0)
	{
		printf("N and M must be positive\n");
		exit(1);
	}
	pid_t makerPid[N];
	pid_t providerPid[N];
	makeSem();
	makeMem();

	for (int i =0; i < N; i++)
	{
		pid_t child = fork();
		if (child == 0)
		{
			execlp("./maker", "maker", NULL);
		}
		makerPid[i] = child;
	}

	for (int i =0; i < M; i++)
	{
		pid_t child = fork();
		if (child == 0)
		{
			//execlp("./provider", "provider", NULL);
		}
		providerPid[i] = child;
	}
	sleep(15);
	return 0;
}
