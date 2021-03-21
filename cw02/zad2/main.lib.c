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
    fprintf(result,"Lib Time: \n");
    fprintf(result, "Real time: %lf \n",timeDiff(ending,begining));
    fprintf(result,"User time: %lf \n", (timeDiff(tEnd->tms_utime,tBegin->tms_utime)));
    fprintf(result, "System time: %lf \n", timeDiff(tEnd->tms_stime, tBegin->tms_stime));
    
    printf("Real time: %lf \n", timeDiff(ending,begining));
    printf("User time: %lf \n", (timeDiff(tEnd->tms_utime,tBegin->tms_utime)));
    printf("System time: %lf \n", timeDiff(tEnd->tms_stime, tBegin->tms_stime));
}
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
		    int operations = 2;
	    clock_t opTime[operations] ;
	    struct tms* Times[operations];

	    for (int i = 0; i < operations ; i++)
	    {
	    	opTime[i] = 0;
	    	Times[i] = calloc(1, sizeof(struct tms*));
	    }
	FILE* fp1;
	fp1 = fopen(name2,"r");
	result = fopen("pomiar_zad_2.txt","a");
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
		opTime[0] = times(Times[0]);
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
		opTime[1] = times(Times[1]);
	saveTime(opTime[0],opTime[1],
		Times[0],Times[1]);

	free(name2);
	fclose(result);
	return 0;
}
