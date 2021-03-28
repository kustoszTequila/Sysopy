#include "bloclib.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/resource.h>
#include<sys/wait.h>

FILE * result;

int getEndIndex(char** argv,int startI,int size)
{	
	int endI = startI;

	while ( endI <= size)
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
	char buffor;
	
	int cutIndex = 0;
	int j = 0;
	
	for (int i = startI; i < endI+1; i++)
	{
		buffor = argv[i][0];
		
		while (buffor != ':')
		{
		    
			cutIndex++;
			buffor = argv[i][cutIndex];
			
		}
		newArgv[2*j] = calloc(1, cutIndex );
		strncpy(newArgv[2*j],argv[i], cutIndex);		
		newArgv[2*j+1] = calloc(1,strlen(argv[i]) - cutIndex );
		strncpy(newArgv[2*j+1],argv[i] + cutIndex + 1,strlen(argv[i]) - cutIndex - 1);

		j++;
		cutIndex = 0;
	}
	
	return newArgv;
}
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


int main(int argc, char** argv) 
{

    int operations = argc;
    clock_t opTime[operations] ;
    struct tms* Times[operations];
    pid_t mainPid = getpid();
    printf("MAIN PID: %d\n",(int)mainPid); 
    for (int i = 0; i < operations ; i++)
    {
    	opTime[i] = 0;
    	Times[i] = calloc(1, sizeof(struct tms*));
    }
    
    struct BlockArray* bArray = NULL;
    struct Sequence* seq = NULL;
    
    if ( argc < 2)
    {
    	printf("Not enough arguments");
    	exit(0);
    }
    
    result = fopen("raport2Pro.txt", "a");
    
    int time_index = 0;
    for (int i = 2; i <argc; i++)
    {
    	opTime[time_index] = times(Times[time_index]);
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
		if (seq != NULL)
			free(seq);
		pid_t child_pid;
		int startI = i + 1;
		int endI = getEndIndex(argv,startI,argc-1);		
		int siz = endI - startI + 1;
		char** newAr = parse(argv,startI,endI);
		seq = createSequence(newAr,2*siz);
		free(newAr);
    		printf("Merging: %d  files \n",2*siz);
    		fprintf(result,"Mergin: %d files \n",2*siz);
    		int status = 0;
		pid_t waitBuffor;
    		for (int j = 0; j < seq->size; j++)
    		{	//printf("ITERACJA: %d  %d\n",(int)getpid(),(int)mainPid);
    			if (getpid() == mainPid)
    			{	
    				printf("Process : %d created\n",(int)child_pid);
    				child_pid = vfork();
    			
    			}
    			if (getppid() == mainPid)
    			{
    				printf("Child Process, pid: %d, Parent Pid: %d\n",(int)getpid(),(int)getppid());
    				fprintf(result,"Child Process, pid: %d, Parent Pid: %d\n",(int)getpid(),(int)getppid());
    				mergeFiles(seq->pairs[j]);
    				exit(0);
    			}	
    			
    			while ((waitBuffor = wait(&status)) > 0);		  		
    		}
    		opTime[time_index] = times(Times[time_index]);
		saveTime(opTime[time_index-1],opTime[time_index],
		Times[time_index-1],Times[time_index]);
		time_index++;
		i = i  + siz ;
	
	}
	else if (strcmp(argv[i], "add_block") == 0)
	{
	    	printf("Saving to : %d  block \n",atoi(argv[i+1]));
    		fprintf(result,"Saving to : %d  blocks \n",atoi(argv[i+1]));
    		addBlock(bArray, seq->pairs[atoi(argv[i+1])]->margedName,seq->pairs[atoi(argv[i+1])]->size);
	    	opTime[time_index] = times(Times[time_index]);
		saveTime(opTime[time_index-1],opTime[time_index],
		Times[time_index-1],Times[time_index]);
		time_index++;
		i = i  + 1;
		//printf("Saved Correctly\n");
	}
	else if (strcmp(argv[i], "remove_block") == 0)
	{
		printf("Removing block of index: %d \n",atoi(argv[i+1]));
		fprintf(result, "Removing block of index: %d \n",atoi(argv[i+1]));
		deleteBlock(bArray,atoi(argv[i+1]));
		i++;
		opTime[time_index] = times(Times[time_index]);
		saveTime(opTime[time_index-1],opTime[time_index],
		Times[time_index-1],Times[time_index]);
    		time_index++;
	}
	else if (strcmp(argv[i], "remove_row") == 0)
	{
		printf("Removing row: %d in block: %d \n",atoi(argv[i+2]),atoi(argv[i+1]));
		fprintf(result, "Removing row: %d in block: %d \n",atoi(argv[i+2]),atoi(argv[i+1]));
		deleteLine(bArray->blocks[atoi(argv[i+1])],atoi(argv[i+2]));
		i = i+1;
		
		opTime[time_index] = times(Times[time_index]);
		saveTime(opTime[time_index-1],opTime[time_index],
		Times[time_index-1],Times[time_index]);
	    	time_index++;
	}
	else
	{
		printf("Wrong argument!\n");	
	}
	
	}
	fprintf(result,"---END--- \n");
	fclose(result);	
    return 0;
}
