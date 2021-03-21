#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <errno.h>
#include <math.h>
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
		    int operations = 2;
	    clock_t opTime[operations] ;
	    struct tms* Times[operations];

	    for (int i = 0; i < operations ; i++)
	    {
	    	opTime[i] = 0;
	    	Times[i] = calloc(1, sizeof(struct tms*));
	    }
	FILE* fp1;
	FILE* fp2;
	fp1 = fopen(name1,"r");
	fp2 = fopen(name2,"a");
	result = fopen("pomiar_zad_5.txt","a");
	if (fp1 ==NULL || fp2 == NULL)
	{
		perror("Error while opening the files");
		exit(-1);
	}
	
	size_t len = 0;
	ssize_t read;
	char * line = NULL;
	char * line2 = calloc(1,51);
	if (line2 == NULL)
	{
		perror("Błąd alokacji");
		exit(-1);
	}
		opTime[0] = times(Times[0]);
	while((read = getline(&line, &len, fp1) != -1))
	{
		if(strlen(line) >50)
		{
			int j = ceil(strlen(line) / 50.0);
			int i = 0;
			while (j >0)
			{
			strncpy(line2,line + i*50,(i+1)*50 - i*50);
			fprintf(fp2,"%s\n",line2);
			j--;
			i++;
			}
			
		}
		else
		{
			fprintf(fp2,"%s",line);
		
		}
	}

	fclose(fp1);
		opTime[1] = times(Times[1]);
	saveTime(opTime[0],opTime[1],
		Times[0],Times[1]);
	fclose(fp2);
	fclose(result);
	free(name2);
	free(name1);
	return 0;
}
