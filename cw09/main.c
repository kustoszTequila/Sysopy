#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#define ELFS 10
#define REINDEER 9
#define MAX_WAIT 3

int problems = 0;
int waitingRein = 0;
int round_ = 0;
int elvesID[MAX_WAIT];

pthread_mutex_t santaMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t elfMutex= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t elfMutex2= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t reindeerMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t reindeerBack = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t santaWait = PTHREAD_COND_INITIALIZER;
pthread_cond_t elfWait = PTHREAD_COND_INITIALIZER;
pthread_cond_t reinWait = PTHREAD_COND_INITIALIZER;

void* santaJob (void* arg)
{
    while(round_ != 3) // we're making 3 rounds
    {
        // sleeping 
        if (pthread_mutex_lock(&santaMutex) != 0)
        {
            perror("Error while locking mutex\n");
            exit(1);
        }
        while(waitingRein < REINDEER && problems < MAX_WAIT)
        {
            pthread_cond_wait(&santaWait,&santaMutex);
        }
        if (pthread_mutex_unlock(&santaMutex) != 0)
        {
            perror("Error while unlocking mutex\n");
            exit(1);
        }
        // woke up
        printf("Mikołaj: budzę się\n");

        // check the reindeers
        if ( pthread_mutex_lock(&reindeerBack) != 0)
        {
            perror("Error while locking mutex\n");
            exit(1);
        }
        if(waitingRein == REINDEER)
        {
            printf("Mikołaj: dostarczam zabawki\n");
            sleep(rand()% 4 + 2);
            round_  ++; // we make another round_ 
            waitingRein = 0; // clean the variable

            pthread_mutex_lock(&reindeerMutex);            
            pthread_cond_broadcast(&reinWait);
            pthread_mutex_unlock(&reindeerMutex);
        }
        if (pthread_mutex_unlock(&reindeerBack) != 0)
        {
            perror("Error while locking mutex\n");
            exit(1);
        }

        // help the elves
        if ( pthread_mutex_lock(&elfMutex2) != 0)
        {
            perror("Error while locking mutex\n");
            exit(1);
        }
        if(problems == MAX_WAIT)
        {
            printf("Mikołaj: rozwiązuje problemy elfów: %d %d %d\n"
            ,elvesID[0],elvesID[1],elvesID[2]);
            sleep(rand()% 2 + 1);

            pthread_mutex_lock(&elfMutex);
            problems = 0;
            for (int i = 0; i < MAX_WAIT; i++)
            {
                elvesID[i] = 0;
            }
            pthread_cond_broadcast(&elfWait);
            pthread_mutex_unlock(&elfMutex);
        }
        printf("Mikołaj: zasypiam\n");
        if (pthread_mutex_unlock(&elfMutex2) != 0)
        {
            perror("Error while unlocking mutex\n");
            exit(1);
        }     
        // back to sleep 
    }
    exit(0);
}
void *elfJob (void* arg)
{
    int ID = *((int*) arg);
    while(1)
    {
        // working ;_;
        sleep(rand() % 5 + 2 ); 

        if (pthread_mutex_lock(&elfMutex) != 0)
        {
            perror("Error while locking mutex\n");
            exit(1);
        }
        while(problems == 3)
        {
            printf("Elf: czeka na powrót elfów, %d\n",ID);
            pthread_cond_wait(&elfWait,&elfMutex);
        }
        if (pthread_mutex_unlock(&elfMutex) != 0)
        {
            perror("Error while locking mutex\n");
            exit(1);
        }

        // report a problem
        if( pthread_mutex_lock(&elfMutex2) != 0)
        {
            perror("Error while locking mutex\n");
            exit(1);
        }
        if (problems < MAX_WAIT)
        {
            
            elvesID[problems] = ID;
            problems++;
            printf("Elf: czeka %d elfów na Mikołaja, %d\n",problems,ID);

            if (problems == MAX_WAIT)
            {              
                pthread_mutex_lock(&santaMutex);
                printf("Elf: wybudzam Mikołaja, %d\n",ID);
                pthread_cond_broadcast(&santaWait);
                pthread_mutex_unlock(&santaMutex);
            }
        }
        pthread_mutex_unlock(&elfMutex2);

    }
}
void *reinJob (void* arg)
{
    int ID = *((int*) arg);
    while(1)
    {
        if (pthread_mutex_lock(&reindeerMutex) != 0)
        {
            perror("Error while locking mutex\n");
            exit(1);
        }
        while(waitingRein > 0 )
        {
            pthread_cond_wait(&reinWait,&reindeerMutex);
        }
        if ( pthread_mutex_unlock(&reindeerMutex) != 0)
        {
            perror("Error while unlocking mutex\n");
            exit(1);
        }
        // less go on holiday :D
        sleep(rand() % 10 + 5);

        // going back :/
        if ( pthread_mutex_lock(&reindeerBack) != 0)
        {
            perror("Error while locking mutex\n");
            exit(1);
        }
        waitingRein++;
        printf("Renifer: czeka %d reniferów na Mikołaja, %d\n",waitingRein,ID);
        if (waitingRein == REINDEER)
        {
            pthread_mutex_lock(&santaMutex);
            printf("Renifer: wybudzam Mikołaja, %d\n",ID);
            pthread_cond_broadcast(&santaWait);
            pthread_mutex_unlock(&santaMutex);
        }
        if (pthread_mutex_unlock(&reindeerBack) != 0 )
        {
            perror("Error while unlocking mutex\n");
            exit(1);
        }

    }
}
int main(int argc, char** argv) 
{
    // create santa's thread
    pthread_t santa;
    if (pthread_create(&santa,NULL,santaJob,NULL) < 0)
    {
        perror("Error while creating santa's thread\n");
        exit(1);
    }

    srand(time(NULL)); // start randomizing
    // create elves' threads
    pthread_t* elves = calloc(ELFS,sizeof(pthread_t));
    for (int i = 0; i < ELFS; i++)
    {
        int *ID = malloc(sizeof(int));
        *ID = (i+1);
        if ((elves[i] = pthread_create(&elves[i],NULL,&elfJob,ID)) < 0 )
        {
            perror("Error while creating elf thread\n");
            exit(1);
        }
    }
    // create reindeer's threads
    pthread_t *reindeers = calloc(REINDEER,sizeof(pthread_t));
     for (int i = 0; i < REINDEER; i++)
    {
        int *ID = malloc(sizeof(int));
        *ID = (i+1);
        if(( reindeers [i] = pthread_create(&reindeers[i],NULL,&reinJob,ID)))
        {
            perror("Error while creating elf thread\n");
            exit(1);
        }
    }   
    // wait for threads to finish
    pthread_join(santa,NULL);
    for (int i = 0; i < ELFS; i++)
    {
        pthread_join(elves[i],NULL);
    }
    for (int i = 0; i < REINDEER; i++)
    {
        pthread_join(reindeers[i],NULL);
    }
    
    // free memory
    free(elves);
    free(reindeers);
    
    return 0;
}