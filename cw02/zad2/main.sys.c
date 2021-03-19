#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <errno.h>

int main (int arg, char** argv)
{

	char* name2;
	char* sign;
	if (arg < 3 )
	{

		perror("Not enough arguments!\n");
		exit(-1);
	}
	if (strlen(argv[1]) != 1)
	{
		perror("First input must be char!\n");
		exit(-1);	
	}
	else
	{
		sign = calloc(1,1);
		name2 = calloc(1, 1 + strlen(argv[2]));		
		strcpy(name2,argv[2]);
		strcpy(sign,argv[1]);
	
	}
	if (sign == NULL || name2 == NULL)
	{
		perror("Error while saving files' names");
		exit(-1);
	}
	int fp1;
	fp1 = open(name2,O_RDONLY);
	if (fp1 == 0 )
	{
		perror("Error while opening the files");
		exit(-1);
	}
	char charac ;
	char * line = calloc(1,256);
	int size = 0;
	if (line == NULL)
	{
		perror("Błąd alokacji");
		exit(-1);
	}
	while (read(fp1,&charac,1) > 0)
	{	line[size] = charac;
		size++;
		if ( charac == '\n')
		{
			for (int i=0;i<size;i++)
			{
				//printf("%c\n",line[i]);
				if (line[i] == sign[0])
				{
					printf("%s",line);
					break;
				}			
			}
			free(line);
			line = calloc(1,256);
			size = 0;
		}
	}

	close(fp1);
	free(name2);
	free(line);
	return 0;
}
