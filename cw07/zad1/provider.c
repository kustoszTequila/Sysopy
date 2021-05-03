#include "pizza.h"

int semId;
int memId;


void setSemId()
{
    key_t key = ftok(getenv("HOME"),'A');
    semId = semget(key,0,0);
    if (semId < 0)
    {
        perror("(Provider) Error while getting semId \n");
        exit(1);
    } 
}
void setMemId()
{
    key_t key = ftok(getenv("HOME"),'B');
    memId = shmget(key,0,0);
    if (memId < 0)
    {
        perror("(Provider) Error while getting memId \n");
        exit(1);
    } 
}
void work()
{
    // zabieramy pizze ze stołu (jeśli jest)
    struct sembuf* startSem = calloc(3,sizeof(struct sembuf));
    startSem[0].sem_num = 4;
    startSem[0].sem_op = -1;
    startSem[0].sem_flg = 0;

    startSem[2].sem_num = 3;
    startSem[2].sem_op = 1;
    startSem[2].sem_flg = 0;

    startSem[1].sem_num = 1;
    startSem[1].sem_op = 1;
    startSem[1].sem_flg = 0;
    
    semop(semId,startSem,3);  
     
    // bierzemy ja z tablicy
    struct pizza *piz = shmat(memId, NULL, 0);
    int indeks = (semctl(semId,3,GETVAL, NULL)) - 1;
    indeks = indeks % CAPACITY;
    int num = piz->number[indeks];
    int pizzas = (semctl(semId,4,GETVAL,NULL));
    printf("(%d %ld) Pobieram pizze: %d Liczba pizz na stole: %d.\n",(int)getpid(),time(0),num,pizzas);
    shmdt(piz);

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
