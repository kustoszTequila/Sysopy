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
#include <sys/shm.h>
#include <sys/wait.h>

#define CAPACITY 5 // Max capacity of oven 
#define SEMNUM 5 // number of semators
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
