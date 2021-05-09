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
	if (key == -1)
	{
		perror("Error with ftok\n");
		exit(1);
	}
	semId = semget(key,(SEMNUM),IPC_CREAT | 0666);
	if (semId < 0 )
	{
		perror("ERROR while creating semaphore\n");
		exit(1);
	}

	union semun arg;
	arg.val = 0;
	if ( semctl(semId,2,SETVAL,arg) < 0)// index for maker
	{
		perror("Error with semctl\n");
		exit(1);
	} 
	if (semctl(semId,3,SETVAL,arg) < 0 ) // index for provider
	{
		perror("Error with semctl\n");
		exit(1);
	} 
	if (semctl(semId,4,SETVAL,arg) < 0) // table (provider's side)
	{
		perror("Error with semctl\n");
		exit(1);
	} 
	arg.val = 5;
	if ( semctl(semId,0,SETVAL,arg) < 0 )// piec
	{
		perror("Error with semctl\n");
		exit(1);
	} 
	if (semctl(semId,1,SETVAL,arg) < 0) // table (maker's side)
	{
		perror("Error with semctl\n");
		exit(1);
	} 

}
void makeMem()
{
	key_t key = ftok(getenv("HOME"),'B');
	if (key < 0)
	{
		perror("Error with ftok\n");
		exit(1);
	}
	memId = shmget(key,sizeof(struct pizza),IPC_CREAT | 0666);
	if (memId < 0)
	{
		printf("Error while creating memory\n");
		exit(1);		
	}
}
void clear()
{
    if (semctl(semId, 0, IPC_RMID, NULL) < 0)
	{		
		perror("Error with semctl\n");
		exit(1);		
	}
    if (shmctl(memId, IPC_RMID, NULL) < 0 )
	{
		perror("Error with ftok\n");
		exit(1);
	}
	
}
void handler(int signum)
{
	clear();
	exit(1);
}
int main (int argc, char **argv)
{
	signal(SIGINT,handler);
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

	makeSem();
	makeMem();

	for (int i =0; i < N; i++)
	{
		pid_t child = fork();
		
		if (child == 0)
		{
			
			execlp("./maker", "maker", NULL);
			
		}
		sleep(1);
	}

	for (int i =0; i < M; i++)
	{
		pid_t child = fork();
		if (child == 0)
		{
			execlp("./provider", "provider", NULL);
		}
		sleep(1);
	}
	for(int i =0;i < N+M; i++)
	{
		wait(NULL);
	}
	clear();
	printf("END OF MAIN\n");
	return 0;
}
