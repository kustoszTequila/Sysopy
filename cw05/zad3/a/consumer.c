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
	char* name;
	int N;
	if (arg != 4 )
	{
			perror("Zła liczba argumentów!\n");
			exit(-1);
	}
	else 
	{
		pipeName = calloc(1, 1 + strlen(argv[1]));		
		strcpy(pipeName,argv[1]);
		name = calloc(1,1 + strlen(argv[2]));
		strcpy(name,argv[2]);
		N = atoi(argv[3]);
		if (pipeName == NULL || name == NULL)
		{
			perror("Błąd alokacji danych!\n");
			exit(-1);
		}
	}
	FILE* p = fopen(pipeName,"r");
	if (p == NULL)
	{
		perror("Błąd podczas otwierania pliku\n");
		exit(-1);	
	}	
	char *text;
	text = calloc(N+1,sizeof(char));
	char ** storage;
	storage = calloc(100,sizeof(char*));
	for (int i = 0; i < 100; i++)
	{
		storage[i] = calloc(1,sizeof(char*));
	}
	if(storage == NULL || text == NULL)
	{
		perror("Błąd alokacji pamięc\n");
		exit(1);
	}	
	int line;
	char num[2];
	int maxLine = 0;
	int n = 1;
	while(fread(num,sizeof(char),1,p) > 0)
	{
		line = atoi(num);
		n = fread(text,sizeof(char),N,p);
		if (n > 0)		
		{	
			if (line > maxLine)
				maxLine = line;
			//printf("l: %d\n",line);
			//printf("s: %s\n",text);
			if (strlen(storage[line]) <= 1)
			{
				//printf("START %d\n",line);
				storage[line] = (char*) realloc(storage[line],N+1);
				if(storage[line] == NULL)
				{
					perror("Błąd alokacji pamięc\n");
					exit(1);
				}
				strcpy(storage[line],text);
				//printf("%s\n",storage[line]);
			}
			else if (n < N)
			{
				//printf("REALLO N  %d\n",line);
				text[n] = '\0';
				text = realloc(text,n);
				storage[line] = (char*) realloc(storage[line],n + strlen(storage[line]));
				if(storage[line] == NULL)
				{
					perror("Błąd alokacji pamięc\n");
					exit(1);
				}
				strcat(storage[line],text);
				//printf("%s\n",storage[line]);
				text = realloc(text,N);			
			}
			else
			{
				//printf("REALLOC %d\n",line);
				storage[line] = (char*) realloc(storage[line],strlen(text) + 1 + strlen(storage[line]));
				if(storage[line] == NULL)
				{
					perror("Błąd alokacji pamięc\n");
					exit(1);
				}
				strcat(storage[line],text);
				//printf("%s\n",storage[line]);
							
			}
		}		
		sleep(0.5);
	}
	FILE* fp = fopen(name,"w");
	if (fp == NULL)
	{
		printf("Błąd podczas otwierania pliku\n");
		exit(1);
	}
	for (int i = 1; i < maxLine+1; i++)
	{
		fprintf(fp,"%s",storage[i]);
		fprintf(fp,"%s","\n");
		//printf("s: %s\n",storage[i]);
	}
	fclose(fp);
	fclose(p);
	
	return 0;
	
}
