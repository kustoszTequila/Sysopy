#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include<sys/wait.h>
#include <signal.h>
#include <string.h>
#include <stddef.h>

pid_t ctchPid;
int numSig;
int receive;
int queue[100000];
int indeks;
void handlerKill(int signum)
{
	receive++;
}
void handlerQueue(int signum, siginfo_t * info, void * context)
{  
	receive++; 
        queue[indeks] = info->si_value.sival_int;
        indeks = indeks + 1;
}
void handlerReal(int signum)
{
        receive++;
}

int main (int argc, char** argv)
{	
	if (argc < 4)
	{
		printf("za mało argumentów\n");
		exit(1);
	}
	ctchPid = atoi(argv[1]);
	numSig = atoi(argv[2]);
	receive = 0;
	indeks = 0;
        if (strcmp(argv[3], "KILL") == 0)
        {
	    struct sigaction sigAct;
	    sigAct.sa_flags = 0;
	    sigAct.sa_handler= handlerKill;
	    sigfillset(&sigAct.sa_mask);
	    sigdelset(&sigAct.sa_mask, SIGUSR1);

		if (sigaction(SIGUSR1, &sigAct, NULL) != 0)
		{
			printf("error in handler\n");
			exit(1);
		}
		for(int i = 0 ; i < numSig; ++i)
		{
			kill(ctchPid,SIGUSR1);
			while(receive <i)
			{
			}
		}
		kill(ctchPid,SIGUSR2);
		sigsuspend(&sigAct.sa_mask);
        }	
    	else if (strcmp(argv[3], "SIGQUEUE") == 0)
    	{
	    	    struct sigaction sigAct;
    		    union sigval val;
		    sigfillset(&sigAct.sa_mask);
		    sigdelset(&sigAct.sa_mask, SIGUSR1);

		    sigAct.sa_flags = SA_SIGINFO;
		    sigAct.sa_sigaction= handlerQueue;

		if (sigaction(SIGUSR1, &sigAct, NULL) != 0)
		{
			printf("error in handler\n");
			exit(1);
		}
		for(int i = 0 ; i < numSig; ++i)
		{
			sigqueue(ctchPid, SIGUSR1, val);
			while(receive <i)
			{
			}
		}


		    sigqueue(ctchPid, SIGUSR2, val);
		    sigsuspend(&sigAct.sa_mask);
    
		for (int i = 0; i < indeks; i++)
		{
			int in = i + 1;
            		printf("Otrzymałem %d sygnał %d \n", in, queue[i]);
        	}		
    	}
        else if (strcmp(argv[3], "SIGRT") == 0)
        {
	    struct sigaction sigAct;

	    sigfillset(&sigAct.sa_mask);
	    sigdelset(&sigAct.sa_mask, SIGRTMIN);

	    sigAct.sa_flags = 0;
	    sigAct.sa_handler = handlerReal;

		if (sigaction((SIGRTMIN), &sigAct, NULL) != 0)
		{
			printf("error in handler\n");
			exit(1);
		}       
		for(int i = 0 ; i < numSig; ++i)
		{
			kill(ctchPid,SIGRTMIN);
			while(receive <i)
			{
			}
		}

		kill(ctchPid,(SIGRTMIN+1));
		sigsuspend(&sigAct.sa_mask);
        }
	else
	{
		printf("Zły argument\n");
		exit(1);
	}
	printf("Wysłano : %d SIGUSR1 do PID: %d, otrzymano: %d \n",numSig, ctchPid, receive);
	return 0;		
}
