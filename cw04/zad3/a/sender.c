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

    
    if (signum == SIGUSR1)
    {
        receive++;
        sigset_t mask;
    	sigfillset(&mask);
        sigdelset(&mask, SIGUSR1);
        sigdelset(&mask, SIGUSR2);
        sigsuspend(&mask);
    }
    else if (signum == SIGUSR2)
    {
    	sigset_t mask;
    	sigfillset(&mask);
        sigprocmask(SIG_SETMASK, &mask, NULL);
    }
    	else
	{
		printf("Error in handler1 \n");
		exit(1);
	}

}
void handlerQueue(int signum, siginfo_t * info, void * context)
{
    
    if (signum == SIGUSR1)
    {
    	receive++;
    	struct sigaction sigAct;
    	sigfillset(&sigAct.sa_mask);
        sigdelset(&sigAct.sa_mask, SIGUSR1);
        sigdelset(&sigAct.sa_mask, SIGUSR2);
        sigAct.sa_flags = SA_SIGINFO;
        sigAct.sa_sigaction = handlerQueue;
	if (sigaction(SIGUSR1, &sigAct, NULL) != 0) 
	{
		printf("error in queue handler\n");
	}
        queue[indeks] = info->si_value.sival_int;
        indeks = indeks + 1;
        sigsuspend(&sigAct.sa_mask);
    }
    else if (signum == SIGUSR2)
    {
        sigset_t mask;
        sigfillset(&mask);
        sigprocmask(SIG_SETMASK, &mask, NULL);
    }
    	else
	{
		printf("Error in handler1 \n");
		exit(1);
	}

}
void handlerReal(int signum)
{

    
    if (signum == SIGRTMIN)
    {
        receive++;
        sigset_t mask;
    	sigfillset(&mask);
        sigdelset(&mask, SIGRTMIN);
        sigdelset(&mask, (SIGRTMIN+1));
        sigsuspend(&mask);
    }
    else if (signum == (SIGRTMIN+1))
    {
    	sigset_t mask;
    	sigfillset(&mask);
        sigprocmask(SIG_SETMASK, &mask, NULL);
    }
    	else
	{
		printf("Error in handler1 \n");
		exit(1);
	}

}
void waitForSigKill()
{
    struct sigaction sigAct;

    sigfillset(&sigAct.sa_mask);
    sigdelset(&sigAct.sa_mask, SIGUSR1);
    sigdelset(&sigAct.sa_mask, SIGUSR2);

    sigAct.sa_flags = 0;
    sigAct.sa_handler= handlerKill;

        if (sigaction(SIGUSR1, &sigAct, NULL) != 0)
        {
        	printf("error in handler\n");
        	exit(1);
        }
        if (sigaction(SIGUSR2, &sigAct, NULL) != 0)
        {
                printf("error in handler\n");
        	exit(1);
        }

    pause(); 
}
void waitForSigQueue()
{
    struct sigaction sigAct;

    sigfillset(&sigAct.sa_mask);
    sigdelset(&sigAct.sa_mask, SIGUSR1);
    sigdelset(&sigAct.sa_mask, SIGUSR2);

    sigAct.sa_flags = SA_SIGINFO;
    sigAct.sa_sigaction= handlerQueue;

        if (sigaction(SIGUSR1, &sigAct, NULL) != 0)
        {
        	printf("error in handler\n");
        	exit(1);
        }
        if (sigaction(SIGUSR2, &sigAct, NULL) != 0)
        {
                printf("error in handler\n");
        	exit(1);
        }

    pause(); 
}
void waitForSigReal()
{
    struct sigaction sigAct;

    sigfillset(&sigAct.sa_mask);
    sigdelset(&sigAct.sa_mask, SIGRTMIN);
    sigdelset(&sigAct.sa_mask, (SIGRTMIN+1));

    sigAct.sa_flags = 0;
    sigAct.sa_handler = handlerReal;

        if (sigaction((SIGRTMIN), &sigAct, NULL) != 0)
        {
        	printf("error in handler\n");
        	exit(1);
        }
        if (sigaction((SIGRTMIN+1), &sigAct, NULL) != 0)
        {
                printf("error in handler\n");
        	exit(1);
        }

    pause(); 
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
		for(int i = 0 ; i < numSig; i++)
		{
			kill(ctchPid,SIGUSR1);
		}
		sleep(0.3);
		kill(ctchPid,SIGUSR2);
		waitForSigKill();
        }	
    	else if (strcmp(argv[3], "SIGQUEUE") == 0)
    	{
    		union sigval val;
		for(int i = 0 ; i < numSig; i++)
		{
			sigqueue(ctchPid, SIGUSR1, val);
		}
		sleep(0.3);
		sigqueue(ctchPid, SIGUSR2, val);

		waitForSigQueue();
		
		for (int i = 0; i < indeks; i++)
		{
			int in = i + 1;
            		printf("Otrzymałem %d sygnał %d \n", in, queue[i]);
        	}		
    	}
        else if (strcmp(argv[3], "SIGRT") == 0)
        {
		for(int i = 0 ; i < numSig; i++)
		{
			kill(ctchPid,SIGRTMIN);
		}
		sleep(1);
		kill(ctchPid,(SIGRTMIN+1));

		waitForSigReal();
        }
	else
	{
		printf("Zły argument\n");
		exit(1);
	}
	printf("Wysłano : %d SIGUSR1 do PID: %d, otrzymano: %d \n",numSig, ctchPid, receive);
	return 0;		
}
