#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include<sys/wait.h>
#include <signal.h>
#include <string.h>
void recivedSig(int signum)
{
	printf("ja cię ale sygnal w :%d \n",(int)getpid());
}

int main (int argc, char** argv)
{
	if ( argc < 2)
	{
		printf("Not enough arguments\n");
		exit(1);
	}
	int sig_no = SIGUSR1;
	if (strcmp(argv[1], "ignore") == 0)
	{
		signal (sig_no,SIG_IGN);
	}
	else if ( strcmp(argv[1], "handler") == 0)
	{
		signal(sig_no,recivedSig);
	}
	else if (strcmp(argv[1], "mask") == 0)
	{
		sigset_t newmask;
		sigset_t oldmask; 
		sigemptyset(&newmask); 
		sigaddset(&newmask, sig_no); 
		if ( sigprocmask(SIG_BLOCK,&newmask,&oldmask) < 0)
		{
			printf("Nie udało się ustawić blokowania sygnału\n");
			exit(1);
		}
		sigset_t waiting_mask;
		sigpending (&waiting_mask);
		if (sigismember (&waiting_mask, sig_no))
		{
			printf("Sygnał: %d jest blokowany\n",sig_no);
		}
	}
	else if (strcmp(argv[1], "pending") == 0)
	{
		sigset_t newmask;
		sigset_t oldmask; 
		sigemptyset(&newmask); 
		sigaddset(&newmask, sig_no); 
		if ( sigprocmask(SIG_BLOCK,&newmask,&oldmask) < 0)
		{
			printf("Nie udało się ustawić blokowania sygnału\n");
			exit(1);
		}
	}
	else
	{
		printf("Wrong argument!\n");	
		exit(1);
	}
	raise(sig_no);
	execl("./ex",argv[1],NULL);
	

	return 0;		
}
