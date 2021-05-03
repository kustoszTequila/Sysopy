#include "pizza.h"

sem_t *semId[SEMNUM];
int memId;


void setSemId()
{
	for (int i = 0; i < SEMNUM; i++)
	{
		semId[i] = sem_open(SemNames[i],O_RDWR);
		if (semId[i] == SEM_FAILED)
		{
			perror("Error while making semaphore \n");
			exit(1);
		}
	}
}
void setMemId()
{
	memId  = shm_open("/SharedMem", O_RDWR , S_IRWXG | S_IRWXU | S_IRWXO);
	if (memId < 0)
	{
		perror("Error while creating shared memory\n");
		exit(1);
	}
}
void work()
{
    // zabieramy pizze ze stołu (jeśli jest)
    if (sem_wait(semId[4]) < 0)
    {
        perror("(Provider) Error while changing semaphore value \n");
        exit(1);
    }  
    if (sem_post(semId[1]) < 0 || sem_post(semId[3]) < 0 )
    {
        perror("(Provider) Error while changing semaphore value \n");
        exit(1);
    }   
    // bierzemy ja z tablicy
    int indeks; 
    sem_getvalue(semId[3],&indeks);
    indeks = indeks - 1;
    indeks = indeks % CAPACITY;
    int pizzas;
    sem_getvalue(semId[4],&pizzas);
    struct pizza* piz = mmap(NULL,sizeof(struct pizza),PROT_READ,MAP_SHARED,memId,0);
    if (piz == (void*)-1)
    {
        perror("(Provider) Can't get shared memory\n");
        exit(1);
    }
    int num = piz->number[indeks];
    if (munmap(piz,sizeof(struct pizza)) < 0)
    {
        perror("(Provider) Can't unlink shared memory\n");
        exit(1);
    }
    printf("(%d %ld) Pobieram pizze: %d Liczba pizz na stole: %d.\n",(int)getpid(),time(0),num,pizzas);

    usleep(((rand() % (5 - 4 + 1) + 4) * 1000000));
    printf("(%d %ld) Dostarczam pizze: %d \n",(int)getpid(),time(0),num);

    usleep(((rand() % (5 - 4 + 1) + 4) * 1000000)); 
    printf("(%d %ld) Wróciłem\n",(int)getpid(),time(0));
}
int main (int argc, char **argv)
{
    setSemId();
    setMemId();
    
    while(1)
    {
        work();
    }
	return 0;
}
