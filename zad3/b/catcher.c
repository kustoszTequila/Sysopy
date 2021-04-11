#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include<sys/wait.h>
#include <signal.h>
#include <string.h>
#include <stddef.h>

int numSig;
pid_t sndPid;

void catchRealHandler(int signum, siginfo_t * info, void * context)
{
    sndPid = info->si_pid;
    if (signum == SIGRTMIN)
    {
    	
    	numSig++;
    	struct sigaction sigAct;
    	sigfillset(&sigAct.sa_mask);
        sigAct.sa_flags = SG_SIGINFO;
        sigAct.sa_sigaction = catchRealHandler;
        sigdelset(&sigAct.sa_mask, SIGRTMIN);
        sigdelset(&sigAct.sa_mask, SIGRTMIN+1);

	
        if (sigaction(SIGRTMIN, &sigAct, NULL) != 0)
        {
        	printf("error in handler\n");
        	exit(1);
        }
        if (sigaction(SIGRTMIN+1, &sigAct, NULL) != 0)
        {
                printf("error in handler\n");
        	exit(1);
        }
	kill(sndPId,(SIGRTMIN+1));
        sigsuspend(&sigAct.sa_mask);
    }
    else if (signum == (SIGRTMIN+1))
    {
	kill(sndPId,(SIGRTMIN+1));
    }
    	else
	{
		printf("Wrong singal \n");
		exit(1);
	}
}
void catchKillHandler(int signum)
{
    if (signum == SIGUSR1)
    {
    	numSig++;
        sigset_t mask;
    	sigfillset(&mask);
        sigdelset(&mask, SIGUSR1);
        sigdelset(&mask, SIGUSR2);
        kill(sndPid,SIGUSR1);
        sigsuspend(&mask);
    }
    else if (signum == SIGUSR2)
	kill(sndPid,SIGUSR1);
    	else
	{
		printf("Wrong singal \n");
		exit(1);
	}
}
void catchKillHandlerFirst(int signum, siginfo_t * info, void * context)
{
    if (signum == SIGUSR1)
    {
    	sndPid = info->si_pid;
    	numSig++;
    	struct sigaction sigAct;
    	sigfillset(&sigAct.sa_mask);
        sigdelset(&sigAct.sa_mask, SIGUSR1);
        sigdelset(&sigAct.sa_mask, SIGUSR2);
        sigAct.sa_flags = 0;
        sigAct.sa_handler = atchKillHandler;
	
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
	kill(sndPid,SIGUSR1);
        sigsuspend(&sigAct.sa_mask);
    }
    else if (signum == SIGUSR2)
	    kill(sndPid,SIGUSR1);
    	else
	{
		printf("Wrong singal \n");
		exit(1);
	}
}
void setKill()
{
    printf("Catcher PID: %d \n",(int)getpid());

    struct sigaction sigAction;

    sigfillset(&sigAction.sa_mask);
    sigdelset(&sigAction.sa_mask, SIGUSR1);
    sigdelset(&sigAction.sa_mask, SIGUSR2);

    sigAction.sa_flags = SA_SIGINFO;
    sigAction.sa_sigaction = catchKillHandlerFirst;

    if (sigaction(SIGUSR1, &sigAction, NULL) != 0)
    {
        	printf("error while setting signal\n");
        	exit(1);
    }
    if (sigaction(SIGUSR2, &sigAction, NULL) != 0)
    {
            	printf("error while setting signal\n");
        	exit(1);
    }

    sigsuspend(&sigAction.sa_mask);
}
void setReal()
{
    printf("Catcher PID: %d \n",(int)getpid());

    struct sigaction sigAction;

    sigfillset(&sigAction.sa_mask);
    sigdelset(&sigAction.sa_mask, SIGRTMIN);
    sigdelset(&sigAction.sa_mask, (SIGRTMIN+1));
    sigAction.sa_flags = SA_SIGINFO;
    sigAction.sa_sigaction = catchRealHandler;


    sigaction(SIGRTMIN, &sigAction NULL);
    sigaction(SIGRTMAX, &sigAction, NULL);

    sigsuspend(&sigAction.sa_mask);
}
int main (int argc, char** argv)
{	
	if (argc < 2)
	{
		printf("za mało argumentów\n");
		exit(1);
	}
	numSig = 0;
        if (strcmp(argv[1], "KILL") == 0)
        {
		setKill();
        }	
    	else if (strcmp(argv[1], "SIGQUEUE") == 0)
    	{
		setQueue();
    	}
        else if (strcmp(argv[1], "SIGRT") == 0)
        {
		setReal();

        }
	else
	{
		printf("Zły argument\n");
		exit(1);
	}

	return 0;		
}
