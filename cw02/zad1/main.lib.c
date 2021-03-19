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
	FILE* fp1;
	FILE* fp2;
	fp1 = fopen(name1,"r");
	fp2 = fopen(name2,"r");
	if (fp1 ==NULL || fp2 == NULL)
	{
		perror("Error while opening the files");
		exit(-1);
	}
	

	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	
	int eof1 = 0;
	int eof2 = 0;
	
	while (eof1 != 1 || eof2 !=1)
	{
	 if ( (read = getline(&line, &len, fp1) != -1))
	 {
	 	printf("%s",line);
	 }	 	
	 else
	 eof1 =1;
	 if ( (read = getline(&line, &len, fp2) != -1))
	 {
	 	printf("%s",line);
	 }
	 else 
	 eof2 = 1;

	}

	fclose(fp1);
	fclose(fp2);
	free(name1);
	free(name2);
	return 0;
}
