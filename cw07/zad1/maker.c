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
        perror("(Maker) Error while getting semId \n");
        exit(1);
    } 
}
void setMemId()
{
    key_t key = ftok(getenv("HOME"),'B');
    memId = shmget(key,0,0);
    if (memId < 0)
    {
        perror("(Maker) Error while getting memId \n");
        exit(1);
    } 
}
void work(int num)
{
    printf("(%d %ld) Przygotowuje pizze: %d\n",(int)getpid(),time(0),num);
    sleep(2);
    // zabieramy jedno miejsce z pieca
    struct sembuf* startSem = calloc(1,sizeof(struct sembuf));
    startSem->sem_num = 1;
    startSem->sem_op = -1;
    startSem->sem_flg = 0;
    semop(semId,startSem,1);
    int amount = 5 - (semctl(semId,1,GETVAL,NULL));
    printf("(%d %ld) Dodałem pizze: %d. Liczba pizz w piecu: %d\n",(int)getpid(),time(0),num,amount);
    sleep(5);
    // oddajemy jedno miejsce w piecu
    struct sembuf* midSem = calloc(1,sizeof(struct sembuf));
    midSem->sem_num = 1;
    midSem->sem_op = 1;
    midSem->sem_flg = 0;
    semop(semId,startSem,1);
    // zabieramy jedno miejsce ze stołu
    struct sembuf* endSem = calloc(3,sizeof(struct sembuf));
    endSem[0].sem_num = 2;
    endSem[0].sem_op =  -1;
    endSem[0].sem_flg = 0;

    endSem[1].sem_num = 5;
    endSem[1].sem_op =  1;
    endSem[1].sem_flg = 0;

    endSem[2].sem_num = 3;
    endSem[2].sem_op =  1;
    endSem[2].sem_flg = 0;
    semop(semId,startSem,3);  
    // dodajemy pizze do tablicy 
    struct pizza *piz = shmat(memId, NULL, 0);
    int indeks = (semctl(semId,3,GETVAL, NULL)) - 1;
    indeks = indeks % CAPACITY;
    piz->number[indeks] = num;
    // Sprawdzamy ile pizz jest w piecu);
    amount = 5 - (semctl(semId,1,GETVAL,NULL));
    int table = 5 - (semctl(semId,2,GETVAL,NULL));
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