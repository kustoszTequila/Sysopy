#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include<sys/wait.h>
#include <signal.h>
#include <string.h>
#include <stddef.h>

int numSig;
pid_t sndPid;
void realCatchHandler(int signum)
{
    if (signum == SIGRTMIN)
    {
    	numSig++;
    	struct sigaction sigAct;
    	sigfillset(&sigAct.sa_mask);
        sigdelset(&sigAct.sa_mask, SIGRTMIN);
        sigdelset(&sigAct.sa_mask, SIGRTMIN+1);
        sigAct.sa_flags = 0;
        sigAct.sa_handler = realCatchHandler;

        sigsuspend(&sigAct.sa_mask);
    }
    else if (signum == (SIGRTMIN+1))
    {
        sigset_t mask;
        sigfillset(&mask);
        sigprocmask(SIG_SETMASK, &mask, NULL);
    }
    	else
	{
		printf("Wrong singal \n");
		exit(1);
	}
}
void realCatchHandlerFirst(int signum, siginfo_t * info, void * context)
{
    if (signum == SIGRTMIN)
    {
    	sndPid = info->si_pid;
    	numSig++;
    	struct sigaction sigAct;
    	sigfillset(&sigAct.sa_mask);
        sigdelset(&sigAct.sa_mask, SIGRTMIN);
        sigdelset(&sigAct.sa_mask, SIGRTMIN+1);
        sigAct.sa_flags = 0;
        sigAct.sa_handler = realCatchHandler;
	
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

        sigsuspend(&sigAct.sa_mask);
    }
    else if (signum == (SIGRTMIN+1))
    {
        sndPid = info->si_pid;
        sigset_t mask;
        sigfillset(&mask);
        sigprocmask(SIG_SETMASK, &mask, NULL);
    }
    	else
	{
		printf("Wrong singal \n");
		exit(1);
	}
}
void catchHandler(int signum)
{
    if (signum == SIGUSR1)
    {
    	numSig++;
    	struct sigaction sigAct;
    	sigfillset(&sigAct.sa_mask);
        sigdelset(&sigAct.sa_mask, SIGUSR1);
        sigdelset(&sigAct.sa_mask, SIGUSR2);
        sigAct.sa_flags = 0;
        sigAct.sa_handler = catchHandler;
	
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
		printf("Wrong singal \n");
		exit(1);
	}
}
void catchHandlerFirst(int signum, siginfo_t * info, void * context)
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
        sigAct.sa_handler = catchHandler;
	
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

        sigsuspend(&sigAct.sa_mask);
    }
    else if (signum == SIGUSR2)
    {
        sndPid = info->si_pid;
        sigset_t mask;
        sigfillset(&mask);
        sigprocmask(SIG_SETMASK, &mask, NULL);
    }
    	else
	{
		printf("Wrong singal \n");
		exit(1);
	}
}
void sendKill()
{
	printf("Do wysłania: %d \n",numSig);
	for(int i = 0 ; i < numSig; i++)
	{
		kill(sndPid,SIGUSR1);
	}
	sleep(0.3);
	kill(sndPid,SIGUSR2);
	printf("Otrzymałem: %d SIGUSR1 od: %d \n",numSig,(int)sndPid);
	
}
void sendQueue()
{
    
    	union sigval value;
    	for(int i = 0 ; i < numSig; i++)
	{
		value.sival_int = i;
		sigqueue(sndPid, SIGUSR1, value);
	}
	sleep(0.3);
	kill(sndPid,SIGUSR2);
	printf("Otrzymałem: %d SIGUSR1 od: %d \n",numSig,(int)sndPid);

}
void sendReal()
{
    
    	for(int i = 0 ; i < numSig; i++)
	{
		kill(sndPid,SIGRTMIN);
	}
	sleep(0.5);
	kill(sndPid,(SIGRTMIN+1));
	printf("Otrzymałem: %d SIGRTMIN od: %d \n",numSig,(int)sndPid);

}
void setSignal()
{
    printf("Catcher PID: %d \n",(int)getpid());

    struct sigaction sigAction;

    sigfillset(&sigAction.sa_mask);
    sigdelset(&sigAction.sa_mask, SIGUSR1);
    sigdelset(&sigAction.sa_mask, SIGUSR2);

    sigAction.sa_flags = SA_SIGINFO;
    sigAction.sa_sigaction = catchHandlerFirst;

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

    pause();
}
void setRealSignal()
{
    printf("Catcher PID: %d \n",(int)getpid());

    struct sigaction sigAction;

    sigfillset(&sigAction.sa_mask);
    sigdelset(&sigAction.sa_mask, SIGRTMIN);
    sigdelset(&sigAction.sa_mask, SIGRTMIN+1);

    sigAction.sa_flags = SA_SIGINFO;
    sigAction.sa_sigaction = realCatchHandlerFirst;

    if (sigaction(SIGRTMIN, &sigAction, NULL) != 0)
    {
        	printf("error while setting signal\n");
        	exit(1);
    }
    if (sigaction(SIGRTMIN + 1, &sigAction, NULL) != 0)
    {
            	printf("error while setting signal\n");
        	exit(1);
    }

    pause();
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
		setSignal();
		sendKill();
        }	
    	else if (strcmp(argv[1], "SIGQUEUE") == 0)
    	{
		setSignal();
		sendQueue();
    	}
        else if (strcmp(argv[1], "SIGRT") == 0)
        {
		setRealSignal();
		sendReal();
        }
	else
	{
		printf("Zły argument\n");
		exit(1);
	}

	return 0;		
}
