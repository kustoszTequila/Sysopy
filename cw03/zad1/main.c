#include <sys/resource.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include<sys/wait.h>

int main (int arg, char** argv)
{
	int n;
	if (arg != 2)
	{
		printf("Zła liczba argumentów\n");
		scanf("%d",&n);
	
	}
	else
	n = atoi(argv[1]);

        pid_t child_pid,wpid;
        int status = 0;
        printf("PID glownego programu: %d\n", (int)getpid());
        child_pid = fork();
	for (int i = 0; i <n-1; i++)
	{
	
         if (child_pid != 0)
               {
               child_pid = fork();	
               }
         else
         break;  
            	
	}
	while ((wpid = wait(&status)) > 0);
	if (child_pid == 0)
	{
		printf("Proces macierzysty: %d, Mój PID: %d\n",(int)getppid(),(int)getpid());	
	
	}


	
	return 0;
}
