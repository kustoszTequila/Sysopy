#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/times.h>

#include "bloclib.h"


FILE * result;

int getEndIndex(char** argv,int startI,int size)
{	
	int endI = startI;
	while ( endI < size)
	{
		int tmp = strlen(argv[endI]);
		if ( argv[endI][tmp-1] != 't' || argv[endI][tmp-2] != 'x' ||
		argv[endI][tmp-3] != 't' || argv[endI][tmp-4] != '.' )
		break;
		
		endI++;
	}

	return endI - 1;
}

char** parse(char** argv, int startI,int endI)
{
	int size = endI - startI + 1; // number of pairs
	size = size*2; // number of files

	char** newArgv = calloc(size,sizeof(char*));
	char buffor = malloc(1);
	
	int cutIndex = 0;

	for (int i = 0; i < size/2; i++)
	{
		buffor = argv[i][0];

		while (buffor != ':')
		{
		    
			cutIndex++;
			buffor = argv[i][cutIndex];
		}
		newArgv[2*i] = malloc(1 + cutIndex);
		strncpy(newArgv[2*i],argv[i],cutIndex);

		newArgv[2*i+1] = malloc(strlen(argv[i] - cutIndex ));
		strncpy(newArgv[2*i+1],argv[i] + cutIndex + 1,strlen(argv[i]) - cutIndex - 1);
		
		cutIndex = 0;
	}
	
	return newArgv;
}

void saveTime(clock_t begining, clock_t ending, struct tms* t_begin, struct tms* t_end)
{
    printf("Real time: %fl \n", ((double)(ending - begining) / sysconf(_SC_CLK_TCK)));
    printf("User time: %fl \n", ((double)(t_end->tms_utime - t_begin->tms_utime) / sysconf(_SC_CLK_TCK)));
    printf("System time: %fl \n", ((double)(t_end->tms_stime - t_begin->tms_stime) / sysconf(_SC_CLK_TCK)));

    fprintf(result, "Real time: %fl \n", ((double)(ending - begining) / sysconf(_SC_CLK_TCK)));
    fprintf(result,"User time: %fl \n", ((double)(t_end->tms_utime - t_begin->tms_utime) / sysconf(_SC_CLK_TCK)));
    fprintf(result, "System time: %fl \n", ((double)(t_end->tms_stime - t_begin->tms_stime) / sysconf(_SC_CLK_TCK)));
}


int main(int argc, char** argv) 
{

    int operations = argc;
    clock_t opTime[operations] ;
    struct tms* Times[operations];

    for (int i = 0; i < operations ; i++)
    {
    	opTime[i] = 0;
    	Times[i] = calloc(1, sizeof(struct tms*));
    }
    
    struct BlockArray* bArray;
    
    if ( argc < 2)
    {
    	printf("Not enough arguments");
    	exit(1);
    }
    
    result = fopen("raport2.txt", "w");
    
    int time_index = 0;
    for (int i = 2; i <argc; i++)
    {
    	opTime[time_index] = times (Times[time_index]);
    	time_index++;

	if (strcmp(argv[i], "create_table") == 0)
	{
	printf("Creating table of size: %d \n",atoi(argv[i+1]));
	fprintf(result, "Creating table of size: %d \n",atoi(argv[i+1]));
	bArray = createBlockArray(atoi(argv[i+1]));
	i++;
	
	}
	else if ( strcmp(argv[i], "merge_files") == 0)
	{
	
	
	
	}
	else if (strcmp(argv[i], "remove_block") == 0)
	{
		printf("Removing block of index: %d \n",atoi(argv[i+1]));
		fprintf(result, "Removing block of index: %d \n",atoi(argv[i+1]));
		deleteBlock(bArray,atoi(argv[i+1]));
		i++;
	}
	
	else if (strcmp(argv[i], "remove_row") == 0)
	{
		printf("Removing row: %d in block: %d \n",atoi(argv[i+2]),atoi(argv[i+1]));
		fprintf(result, "Removing row: %d in block: %d \n",atoi(argv[i+2]),atoi(argv[i+1]));
		deleteLine(bArray->blocks[atoi(argv[i+1])],atoi(argv[i+2]));
		i = i+2;
	}
	else
	{
		printf("Wrong argument!\n");
	
	}
	opTime[time_index] = times (Times[time_index]);
	saveTime(opTime[time_index-1],opTime[time_index],
	Times[time_index-1],Times[time_index]);
    	time_index++;
	
	}
	fclose(result);	
    return 0;
}
