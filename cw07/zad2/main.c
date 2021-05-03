#include "pizza.h"

void makeSem()
{
	sem_t* semaphore;
	for (int i = 0; i < 2; i++)
	{
		semaphore = sem_open(SemNames[i],O_CREAT | O_RDWR, S_IRWXG | S_IRWXU | S_IRWXO,5 );
		if (semaphore == SEM_FAILED)
		{
			perror("Error while making semaphore with 5\n");
			exit(1);
		}
		sem_close(semaphore);
	}
	for (int i = 2; i < SEMNUM; i++)
	{
		semaphore = sem_open(SemNames[i],O_CREAT | O_RDWR, S_IRWXG | S_IRWXU | S_IRWXO,0);
		if (semaphore == SEM_FAILED)
		{
			perror("Error while making semaphore with 0\n");
			exit(1);
		}
		sem_close(semaphore);
	}
}
void makeMem()
{
	int mem = shm_open("/SharedMem", O_RDWR | O_CREAT, S_IRWXG | S_IRWXU | S_IRWXO);
	if (mem < 0)
	{
		perror("Error while creating shared memory\n");
		exit(1);
	}
	if (ftruncate(mem,sizeof (struct pizza)) < 0)
	{
		perror("Error while truncating memory\n");
		exit(1);
	}
}
void clear()
{
	if (shm_unlink("/SharedMem") < 0)
	{
		perror("Error while unlinking shared memory\n");
		exit(1);
	}
	for (int i = 0; i < SEMNUM; i++)
	{
		if (sem_unlink(SemNames[i]) < 0)
		{
			perror("Error while unlinking semaphore\n");
			exit(1);
		}
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
