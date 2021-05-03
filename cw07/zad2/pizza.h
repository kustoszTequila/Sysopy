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
#include <sys/stat.h>
#include <signal.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/mman.h>

#define CAPACITY 5
#define SEMNUM 5 // number of semators

const char* SemNames[SEMNUM] = {"/OVEN","/TABLE","/MAKE_IND","/PROV_IND","/PROV_TABLE"};
struct pizza // we use in in common memeory
{
	int number[CAPACITY];
};
struct tm* getCurTime() // used for detailed time
{
    time_t rawtime;  
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    return timeinfo;
}
