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
	FILE* fp1;
	fp1 = fopen(name2,"r");
	if (fp1 ==NULL )
	{
		perror("Error while opening the files");
		exit(-1);
	}
	char * line = calloc(1,256);
	if (line == NULL)
	{
		perror("Błąd alokacji");
		exit(-1);
	}
	size_t len = 0;
	ssize_t read;
	while((read = getline(&line, &len, fp1) != -1))
	{
		if(strlen(line) >1)
		{
			for( int i = 0;i<strlen(line) ;i++)
			{

				if (line[i] == sign[0])
				{
					printf("%s",line);
					break;
				}
			}
		}
	}

	fclose(fp1);
	free(name2);
	return 0;
}
