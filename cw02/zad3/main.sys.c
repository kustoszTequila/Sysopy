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

    fprintf(result, "Sys time: \n");
    fprintf(result, "Real time: %lf \n",timeDiff(ending,begining));
    
    fprintf(result,"User time: %lf \n", (timeDiff(tEnd->tms_utime,tBegin->tms_utime)));
    fprintf(result, "System time: %lf \n", timeDiff(tEnd->tms_stime, tBegin->tms_stime));
    
    printf("Real time: %lf \n", timeDiff(ending,begining));
    printf("User time: %lf \n", (timeDiff(tEnd->tms_utime,tBegin->tms_utime)));
    printf("System time: %lf \n", timeDiff(tEnd->tms_stime, tBegin->tms_stime));

}
int fp2,fp3,fp4;

int isEven(int a)
{
	if (a % 2 == 0)
	return 1;
	else
	return 0;
}
int decimal(int a)
{
	int tmp = a/10;
	if (tmp == 0)
		return 0;
	tmp = tmp % 10;
	if (tmp == 7 || tmp == 0)
		return 1;
	return 0;
}
int ifSquare(int a)
{
	if (a <= 0)
		return 0;
	double rt = sqrt((double)a);
	int rt2 = rt;
	if (rt == rt2)
		return 1;
	return 0;

}
int main (int arg, char** argv)
{

	char* name2;
	if (arg < 2 )
	{

		perror("Not enough arguments!\n");
		exit(-1);
	}
	else
	{
		name2 = calloc(1, 1 + strlen(argv[1]));		
		strcpy(name2,argv[1]);
	
	}
	if (name2 == NULL)
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
	int fp1;
	fp1 = open(name2,O_RDONLY);
	fp2 = open("a.txt",O_WRONLY | O_CREAT);
	fp3 = open("b.txt",O_WRONLY | O_CREAT);
	fp4 = open("c.txt",O_WRONLY | O_CREAT);
		result = fopen("pomiar_zad_3.txt","a");
	if (fp1 == 0 || fp2 == 0 || fp3 == 0 || fp4 == 0 )
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
		opTime[0] = times(Times[0]);
	while (read(fp1,&charac,1) > 0)
	{	
		line[size] = charac;
		size++;
		if ( charac == '\n')
		{
			if (strlen(line) > 1)
			{
				int a = atoi(line);
				if(isEven(a) == 1)
				{
					write(fp2,line,strlen(line));
				}
				if(decimal(a) == 1)
				{
					write(fp3,line,strlen(line));
				}
				if(ifSquare(a) == 1)
				{
					write(fp4,line,strlen(line));
				}
					
			}

			free(line);
			line = calloc(1,256);
			size = 0;
		}
	}
	
		opTime[1] = times(Times[1]);
	saveTime(opTime[0],opTime[1],
	Times[0],Times[1]);
	fclose(result);
	close(fp1);
	close(fp2);
	close(fp3);
	close(fp4);

	free(name2);

	free(line);
	return 0;
}
