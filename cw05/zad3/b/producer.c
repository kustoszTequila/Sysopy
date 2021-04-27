#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <ctype.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
int main (int arg, char** argv)
{
	char* pipeName;
	char* line;
	char* name;
	int N;
	if (arg != 5 )
	{
			perror("Zła liczba argumentów!\n");
			exit(-1);
	}
	else 
	{
		pipeName = calloc(1, 1 + strlen(argv[1]));		
		strcpy(pipeName,argv[1]);
		line = calloc(1,1 + strlen(argv[2]));
		strcpy(line,argv[2]);
		name = calloc(1,1 + strlen(argv[3]));
		strcpy(name,argv[3]);
		N = atoi(argv[4]);
		if (pipeName == NULL || line == NULL || name == NULL)
		{
			perror("Błąd alokacji danych!\n");
			exit(-1);
		}
	}
	FILE* p = fopen(pipeName,"w");
	if (p == NULL)
	{
		perror("Błąd podczas otwierania pipe\n");
		exit(1);	
	}	
	FILE* fp = fopen(name,"r");
	if (fp == NULL)
	{
		perror("Błąd podczas otwierania pliku\n");
		exit(1);	
	}	
	char *text;
	text = calloc(N,sizeof(char));
	if (text == NULL)
	{
		perror("Błąd alokacji pamięci\n");
		exit(1);
	}
	int n = fread(text,sizeof(char),N,fp);
	while(n > 0)
	{		
		
		if( n < N-1)
		{
			break;
		}
		fprintf(p,"%s",line);
		fprintf(p,"%s",text);	
		//printf("%s\n",text);
		sleep(0.5);
		n = fread(text,sizeof(char),N,fp);

	}
	fclose(fp);
	fclose(p);
	
	return 0;
	
}
