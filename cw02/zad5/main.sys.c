#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <errno.h>
#include <math.h>

int main (int arg, char** argv)
{
	char* name1,*name2;
	if (arg < 3 )
	{
		perror("Not enough arguments!\n");
		name1 = calloc(1, 256);
		name2 = calloc(1, 256);		
		scanf("%s",name1);
		scanf("%s",name2);
		name1 = realloc(name1,strlen(name1)+1);
		name2 = realloc(name2,strlen(name2)+1);
	}
	else
	{

		name1 = calloc(1, 1 + strlen(argv[1]));
		name2 = calloc(1, 1 + strlen(argv[2]));		
		strcpy(name1,argv[1]);
		strcpy(name2,argv[2]);
	
	}
	if (name1 == NULL || name2 == NULL)
	{
		perror("Error while saving files' names");
		exit(-1);
	}
	int fp1;
	int fp2;
	fp1 = open(name1,O_RDONLY);
	fp2 = open(name2,O_CREAT | O_WRONLY| O_APPEND);
	if (fp1 == 0 || fp2 == 0)
	{
		perror("Error while opening the files");
		exit(-1);
	}
	char charac ;
	char * line = calloc(1,512);
	char* line2 = calloc(1,51);
	int size = 0;
	if (line == NULL)
	{
		perror("Błąd alokacji");
		exit(-1);
	}
	while (read(fp1,&charac,1) > 0)
	{	line[size] = charac;
		size++;
		if ( charac == '\n' )
		{
		if(strlen(line) >50 && strlen(line) > 1)
		{
			int j = ceil(strlen(line) / 50.0);
			int i = 0;
			while (j >0)
			{
			
			strncpy(line2,line + i*50,(i+1)*50 - i*50);
			printf("%s\n",line2);
			write(fp2,line2,strlen(line2));
			write(fp2,"\n",1);
			j--;
			i++;
			}
			
		}
		else 
		{
			write(fp2,line,strlen(line));
		}
			free(line);
			line = calloc(1,512);
			size = 0;
		}
	}

	close(fp1);
	close(fp2);
	free(name1);
	free(name2);
	free(line);
	free(line2);
	return 0;
}
