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
	int fp1;
	int fp2;
	fp1 = open(name1,O_RDONLY);
	fp2 = open(name2,O_RDONLY);
	if (fp1 == 0 || fp2 == 0)
	{
		perror("Error while opening the files");
		exit(-1);
	}
	char  line ;
	size_t len = 0;
	int isRead = 0;
	int eof1 = 0;
	int eof2 = 0;
	
	while (eof1 != 1 || eof2 !=1)
	{
	isRead = read(fp1,&line,1);
	if (isRead == 0)
	{
	 	eof1 = 1;
	}
	 while ( isRead > 0)
	 {
	 	if(line == '\n')
	 	{
	 		printf("\n");
	 		break;
	 	}
	 	printf("%c",line);
	 	isRead = read(fp1,&line,1);	 	
	 }
	 // Second file
	 isRead = read(fp2,&line,1);
	if (isRead == 0)
	{
	 	eof2 = 1;
	}
	 while ( isRead > 0)
	 {
	 	if(line == '\n')
	 	{
	 		printf("\n");
	 		break;
	 	}
	 	printf("%c",line);
	 	isRead = read(fp2,&line,1);	 	
	 }
	 	 
	}

	close(fp1);
	close(fp2);
	free(name1);
	free(name2);
	return 0;
}
