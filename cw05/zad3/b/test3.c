#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <ctype.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
int main (int arg, char** argv)
{
	if (arg > 2)
	{
		perror("Zła liczba argumentów\n");
		exit(1);
	}	
	int n = 30;
	if (arg == 2)
		n = atoi(argv[1]);	
	mkfifo("PIPE",0666);
	char N[3];
	sprintf(N,"%d",n);
	pid_t pid = fork();
	if(pid == 0)
	if (execlp("./producer","producer","PIPE","1","pro1.txt",N,NULL) == -1)
	{
		printf("Nie udało sie uruchomić Producenta 1\n");
		exit(1);
	}
	pid = fork();
	if(pid == 0)	
	if (execlp("./consumer","consumer","PIPE","file3.txt",N,NULL) == -1)
	{
		printf("Nie udało sie uruchomić Konsumenta \n");
		exit(1);
	}
	pid = fork();
	if(pid == 0)
	if (execlp("./consumer","consumer","PIPE","file3.txt",N,NULL) == -1)
	{
		printf("Nie udało sie uruchomić Konsumenta 2\n");
		exit(1);
	}
	pid = fork();
	if(pid == 0)
	if (execlp("./consumer","consumer","PIPE","file3.txt",N,NULL) == -1)
	{
		printf("Nie udało sie uruchomić Konsumenta 2\n");
		exit(1);
	}
	pid = fork();
	if(pid == 0)
	if (execlp("./consumer","consumer","PIPE","file3.txt",N,NULL) == -1)
	{
		printf("Nie udało sie uruchomić Konsumenta 2\n");
		exit(1);
	}
	return 0;
}
