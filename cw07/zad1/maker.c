#include "pizza.h"

int semId;
int memId;


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

    usleep(((rand() % (2 - 1 + 1) + 1) * 1000000));
    // zabieramy jedno miejsce z pieca
    struct sembuf* startSem = calloc(1,sizeof(struct sembuf));
    startSem->sem_num = 0;
    startSem->sem_op = -1;
    startSem->sem_flg = 0;
    if (semop(semId,startSem,1) < 0)
    {
        perror("Error with semop\n");
        exit(1);
    }   
    int amount = 5 - (semctl(semId,0,GETVAL,NULL));
    printf("(%d %ld) Dodałem pizze: %d. Liczba pizz w piecu: %d\n",(int)getpid(),time(0),num,amount);

    usleep(((rand() % (5 - 4 + 1) + 4) * 1000000));

    // oddajemy jedno miejsce w piecu
    struct sembuf* midSem = calloc(1,sizeof(struct sembuf));
    midSem->sem_num = 0;
    midSem->sem_op = 1;
    midSem->sem_flg = 0;
    if (semop(semId,midSem,1) < 0)
    {
        perror("Error with semop\n");
        exit(1);
    }
    // zabieramy jedno miejsce ze stołu
    struct sembuf* endSem = calloc(3,sizeof(struct sembuf));
    endSem[2].sem_num = 2;
    endSem[2].sem_op =  1;
    endSem[2].sem_flg = 0;
   
    endSem[0].sem_num = 1;
    endSem[0].sem_op =  -1;
    endSem[0].sem_flg = 0;

    endSem[1].sem_num = 4;
    endSem[1].sem_op =  1;
    endSem[1].sem_flg = 0;
    
    if ( semop(semId,endSem,3) < 0)
    {
        perror("Error with semop");
        exit(1);
    } 
    
    // dodajemy pizze do tablicy 
    struct pizza *piz = shmat(memId, NULL, 0);
    int indeks = (semctl(semId,2,GETVAL, NULL)) - 1;
    indeks = indeks % CAPACITY;
    piz->number[indeks] = num;
    
    // Sprawdzamy ile pizz jest w piecu);
    amount = 5 - (semctl(semId,0,GETVAL,NULL));
    int table = 5 - (semctl(semId,1,GETVAL,NULL));
    printf("(%d %ld) Wyjmuję pizze: %d. Liczba pizz w piecu: %d. Liczba pizz na stole: %d.\n",
    (int)getpid(),time(0),num,amount,table);
    shmdt(piz);
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
