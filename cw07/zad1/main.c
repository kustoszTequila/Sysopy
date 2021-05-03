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
	semctl(semId,2,SETVAL,arg); // index for maker
	semctl(semId,3,SETVAL,arg); // index for provider
	semctl(semId,4,SETVAL,arg); // table (provider's side)
	arg.val = 5;
	semctl(semId,0,SETVAL,arg); // piec
	semctl(semId,1,SETVAL,arg); // table (maker's side)

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
void clear()
{
    semctl(semId, 0, IPC_RMID, NULL);
    shmctl(memId, IPC_RMID, NULL);
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
