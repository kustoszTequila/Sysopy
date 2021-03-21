#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <sys/times.h>
#include <time.h>

FILE* result;

double timeDiff(clock_t end, clock_t begin) {
    return  ((double)(end - begin) / sysconf( _SC_CLK_TCK));
}
void saveTime(clock_t begining, clock_t ending, struct tms* tBegin, struct tms* tEnd)
{
    fprintf(result, "Sys time: \n");
    fprintf(result, "Real time: %lf \n",timeDiff(ending,begining));
    
    fprintf(result,"User time: %lf \n", (timeDiff(tEnd->tms_utime,tBegin->tms_utime)));
    fprintf(result, "System time: %lf \n", timeDiff(tEnd->tms_stime, tBegin->tms_stime));
    
    printf("Real time: %lf \n", timeDiff(ending,begining));
    printf("User time: %lf \n", (timeDiff(tEnd->tms_utime,tBegin->tms_utime)));
    printf("System time: %lf \n", timeDiff(tEnd->tms_stime, tBegin->tms_stime));
}
int main (int arg, char** argv)
{

	char* name1,*name2,*string1,*string2;
	if (arg < 5 )
	{
		perror("Not enough arguments!\n");
		name1 = calloc(1, 256);
		name2 = calloc(1, 256);
		string1 = calloc(1, 256);
		string2 = calloc(1, 256);		
		scanf("%s",name1);
		scanf("%s",name2);
		scanf("%s",string1);
		scanf("%s",string2);
		name1 = realloc(name1,strlen(name1)+1);
		name2 = realloc(name2,strlen(name2)+1);
		string1 = realloc(string1,strlen(string1)+1);
		string2 = realloc(string2,strlen(string2)+1);
	}
	else
	{

		name1 = calloc(1, 1 + strlen(argv[1]));
		name2 = calloc(1, 1 + strlen(argv[2]));	
		string1 = calloc(1, 1 + strlen(argv[3]));	
		string2 = calloc(1, 1 + strlen(argv[4]));		
		strcpy(name1,argv[1]);
		strcpy(name2,argv[2]);
		strcpy(string1,argv[3]);
		strcpy(string2,argv[4]);
	
	}
	if (name1 == NULL || name2 == NULL || string1 == NULL || string2 == NULL)
	{
		perror("Error while saving files' names");
		exit(-1);
	}
		    int operations = 3;
	    clock_t opTime[operations] ;
	    struct tms* Times[operations];

	    for (int i = 0; i < operations ; i++)
	    {
	    	opTime[i] = 0;
	    	Times[i] = calloc(1, sizeof(struct tms*));
	    }
	int fp1,fp2;
	fp1 = open(name1,O_RDONLY);
	fp2 = open(name2,O_CREAT | O_WRONLY| O_APPEND);
	result = fopen("pomiar_zad_4.txt","a");
	if (fp1 == 0 || fp2 == 0)
	{
		perror("Error while opening the files");
		exit(-1);
	}
	char charac ;
	char * line = calloc(1,512);
	char* token;
	int size = 0;
	if (line == NULL)
	{
		perror("Błąd alokacji");
		exit(-1);
	}
	opTime[0] = times(Times[0]);
	while (read(fp1,&charac,1) > 0)
	{	line[size] = charac;
		size++;
		if ( charac == '\n')
		{
		token = strtok(line," ");
		while (token != NULL)
		{
			
			if( strcmp(token,string1) == 0)
				{
					write(fp2,string2,strlen(string2));
					
				}
				
			else
				write(fp2,token,strlen(token));
			write(fp2," ",1);
			token = strtok(NULL," ");
		}
		free(line);
		size = 0;
		line = calloc(1,512);
		if (line == NULL)
		{
			perror("Error while allocating memory");
			exit(-1);
		}
		
		}
	}
	opTime[1] = times(Times[1]);
	saveTime(opTime[0],opTime[1],
	Times[0],Times[1]);
	
	close(fp1);
	close(fp2);
	fclose(result);
	free(name2);
	free(name1);
	free(string1);
	free(string2);
	free(line);
	return 0;
}
