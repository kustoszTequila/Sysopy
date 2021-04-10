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
	if ( argc < 1)
	{
		printf("Not enough arguments in exec\n");
		exit(1);
	}
	if (strcmp(argv[0], "pending") == 0)
	{
		sigset_t waiting_mask;
		sigpending (&waiting_mask);
		if (sigismember (&waiting_mask, SIGUSR1))
		{
			printf("Sygnał: jest blokowany w potomku\n");
		}
	}
	else
	{
		raise(SIGUSR1);				
	}		
}
