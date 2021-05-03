#include "pizza.h"

sem_t *semId[SEMNUM];
int memId;


void setSemId()
{
	for (int i = 0; i < SEMNUM; i++)
	{
		semId[i] = sem_open(SemNames[i],O_RDWR);
		if (semId[i] < 0 )
		{
			perror("Error while making semaphore \n");
			exit(1);
		}
	}
}
void setMemId()
{;
	memId  = shm_open("/SharedMem", O_RDWR , S_IRWXG | S_IRWXU | S_IRWXO);
	if (memId < 0)
	{
		perror("Error while creating shared memory\n");
		exit(1);
	}
}
void work(int num)
{
    printf("(%d %ld) Przygotowuje pizze: %d\n",(int)getpid(),time(0),num);
    usleep(((rand() % (2 - 1 + 1) + 1) * 1000000));
    
    // zabieramy jedno miejsce z pieca
    if (sem_wait(semId[0]) < 0)
    {
        perror("(Maker) Error while decrementing semaphore\n");
        exit(1);
    } 
    int value;  
    sem_getvalue(semId[0],&value);
    int amount = 5 - value;
    printf("(%d %ld) Dodałem pizze: %d. Liczba pizz w piecu: %d\n",(int)getpid(),time(0),num,amount);
    usleep(((rand() % (5 - 4 + 1) + 4) * 1000000));
    
    // oddajemy jedno miejsce w piecu
    if (sem_post(semId[0]) < 0)
    {
        perror("(Maker) Error while incrementing semaphore\n");
        exit(1);
    }
    // zabieramy jedno miejsce ze stołu
    if (sem_post(semId[2]) < 0 )
    {
        perror("(Maker) Error while changing semaphore value \n");
        exit(1);
    }
    if (sem_post(semId[4]) < 0  || (sem_wait(semId[1]) < 0))
    {
        perror("(Maker) Error while changing semaphore value \n");
        exit(1);
    }
    // dodajemy pizze do tablicy 
    int indeks;
    sem_getvalue(semId[2],&indeks);
    indeks = indeks - 1;
    indeks = indeks % CAPACITY;
    struct pizza* piz = mmap(NULL,sizeof(struct pizza),PROT_WRITE | PROT_READ,MAP_SHARED,memId,0);
    if (piz == (void*)-1)
    {
        perror("(Maker) Can't get shared memory\n");
        exit(1);
    }
    piz->number[indeks] = num;  
    if (munmap(piz,sizeof(struct pizza)) < 0)
    {
        perror("(Maker) Can't unlink shared memory\n");
        exit(1);
    }
    // Sprawdzamy ile pizz jest w piecu);
    sem_getvalue(semId[0],&amount);
    amount = 5 - amount;
    int table;
    sem_getvalue(semId[1],&table);
    printf("STOL %d\n",table);
    table = 5 - table;
    printf("(%d %ld) Wyjmuję pizze: %d. Liczba pizz w piecu: %d. Liczba pizz na stole: %d.\n",
    (int)getpid(),time(0),num,amount,table);
}
int main (int argc, char **argv)
{
    setSemId();
    setMemId();
    srand(time(NULL));
    
    while(1)
    {
        int num = (rand() % (9 + 1));
        work(num);
    }
	return 0;
}
