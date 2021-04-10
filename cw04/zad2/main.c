#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include<sys/wait.h>
#include <signal.h>
#include <string.h>
#include <stddef.h>
struct sigaction new_action;
void handler (int sig, siginfo_t *info, void *ucontext)
{
	printf("Numer sygnału: %d\n",(int)info->si_signo);
	printf("Wysłano przez: PID: %ld\n",(long)info->si_pid);
	printf("Wysłano przez: UID: %ld\n",(long)info->si_uid);
	printf("User time consumed: %ld\n",(long)info->si_utime);
	printf("System time consumed: %ld\n",(long)info->si_stime);
	
}

void handler2 (int signum)
{
	printf("Sygnał: %d zaczyna\n",signum);
	sleep(3);
	printf("Sygnał: %d kończy\n",signum);	
}
void handler3 (int signum)
{
	printf("Nowy handler\n");
	
}
int main (int argc, char** argv)
{	
	if (argc < 2)
	{
		printf("za mało argumentów\n");
		exit(1);
	}
	struct sigaction new_action, old_action;
	sigset_t newmask;
	sigemptyset (&new_action.sa_mask);
	sigaddset(&newmask, SIGUSR2);
	if (strcmp(argv[1],"info") == 0)
	{
		new_action.sa_sigaction = handler;
		new_action.sa_mask = newmask;
		new_action.sa_flags = 0;
		sigaction (SIGUSR1, &new_action, &old_action);
		raise(SIGUSR1);
	} 
	else if (strcmp(argv[1],"defer") == 0)
	{
		new_action.sa_handler = handler2;
		new_action.sa_mask = newmask;
		new_action.sa_flags = SA_NODEFER;
		sigaction (SIGINT, &new_action, &old_action);
		raise(SIGINT);
	
	}
	else if (strcmp(argv[1],"reset") == 0)
	{
		new_action.sa_handler = handler3;
		new_action.sa_mask = newmask;
		new_action.sa_flags = SA_RESETHAND;
		sigaction (SIGUSR1, &new_action, &old_action);
		raise(SIGUSR1);
		raise(SIGUSR1);	
	}
	else
	{
		printf("Zły argument\n");
		exit(1);
	}

	return 0;		
}
