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
    struct sembuf* startSem = calloc(2,sizeof(struct sembuf));
    startSem[0].sem_num = 5;
    startSem[0].sem_op = -1;
    startSem[0].sem_flg = 0;

    startSem[2].sem_num = 4;
    startSem[2].sem_op = 1;
    startSem[2].sem_flg = 0;

    startSem[1].sem_num = 2;
    startSem[1].sem_op = 1;
    startSem[1].sem_flg = 0;
    semop(semId,startSem,3);   
    // bierzemy ja z tablicy
    struct pizza *piz = shmat(memId, NULL, 0);
    int indeks = (semctl(semId,4,GETVAL, NULL)) - 1;
    indeks = indeks % CAPACITY;
    int num = piz->number[indeks];
    int pizzas = (semctl(semId,5,GETVAL,NULL)) - 1;
    printf("(%d %ld) Pobieram pizze: %d Liczba pizz na stole: %d.\n",(int)getpid(),time(0),num,pizzas);
    sleep(5);

    printf("(%d %ld) Dostarczam pizze: %n \n",(int)getpid(),time(0),num);
    sleep(5);   
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