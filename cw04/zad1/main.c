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
	for (int i = 1; i < argc; i++)
	{
	if (strcmp(argv[i], "ignore") == 0)
	{
		signal (sig_no,SIG_IGN);
	}
	else if ( strcmp(argv[i], "handler") == 0)
	{
		signal(sig_no,recivedSig);
	}
	else if (strcmp(argv[i], "mask") == 0)
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
	else if (strcmp(argv[i], "pending") == 0)
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
	pid_t childPid;
	if ((childPid = fork()) == -1) 
	{
		printf("Błąd podczas wywoływania sygnału potomnego\n");
	}
	if (childPid ==0)
	{
		if (strcmp(argv[i], "pending") == 0)
		{
			sigset_t waiting_mask;
			sigpending (&waiting_mask);
			if (sigismember (&waiting_mask, sig_no))
			{
				printf("Sygnał: %d jest blokowany w potomku\n",sig_no);
			}
		}
		else
		{
		raise(sig_no);				
		}
		exit(0);
	}
	else
	{
		int status;
		pid_t wpid;
		while ((wpid = wait(&status)) > 0);
	}	
	}

	return 0;		
}
