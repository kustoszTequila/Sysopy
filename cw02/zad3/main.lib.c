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

    fprintf(result, "Real time: %lf \n",timeDiff(ending,begining));
    fprintf(result,"User time: %lf \n", (timeDiff(tEnd->tms_utime,tBegin->tms_utime)));
    fprintf(result, "System time: %lf \n", timeDiff(tEnd->tms_stime, tBegin->tms_stime));
    
    printf("Real time: %lf \n", timeDiff(ending,begining));
    printf("User time: %lf \n", (timeDiff(tEnd->tms_utime,tBegin->tms_utime)));
    printf("System time: %lf \n", timeDiff(tEnd->tms_stime, tBegin->tms_stime));
}
FILE *fp2,*fp3,*fp4;

void isEven(int a)
{
	if (a % 2 == 0)
	fprintf(fp2,"%d\n",a);
}
void decimal(int a)
{
	int tmp = a/10;
	if (tmp == 0)
		return;
	tmp = tmp % 10;
	if (tmp == 7 || tmp == 0)
		fprintf(fp3,"%d\n",a);
}
void ifSquare(int a)
{
	if (a <= 0)
		return;
	double rt = sqrt((double)a);
	int rt2 = rt;
	if (rt == rt2)
		fprintf(fp4,"%d\n",a);

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
	
		    int operations = 2;
	    clock_t opTime[operations] ;
	    struct tms* Times[operations];

	    for (int i = 0; i < operations ; i++)
	    {
	    	opTime[i] = 0;
	    	Times[i] = calloc(1, sizeof(struct tms*));
	    }
	FILE* fp1 = fopen(name2,"r");
	fp1 = fopen(name2,"r");
	fp2 = fopen("a.txt","w");
	fp3 = fopen("b.txt","w");
	fp4 = fopen("c.txt","w");
	result = fopen("pomiar_zad_3.txt","w");
	if (fp1 == NULL || fp2 == NULL || fp3 == NULL || fp4 == NULL )
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
	int j = 0;
	opTime[0] = times(Times[0]);
	while(fscanf(fp1,"%d",&j) == 1)
	{	
		if(j != 0)
		{
		isEven(j);
		decimal(j);
		ifSquare(j);	
		}
	}

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
		opTime[1] = times(Times[1]);
	saveTime(opTime[0],opTime[1],
		Times[0],Times[1]);
	fclose(result);
	free(name2);
	return 0;
}
