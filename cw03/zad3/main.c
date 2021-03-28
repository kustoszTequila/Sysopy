#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include <string.h>
#include<sys/wait.h>
#include <sys/resource.h>
#include <dirent.h>

int isText(char* file)
{	
	char ext[] = ".txt";
	for(int i = 4; i>0;i--)
	{
		if (ext[4-i] != file[strlen(file) -i])
			return 0;
	
	}
	return 1;
	


}

int main (int arg, char** argv)
{
	int n;
	char* startDir;
	char* lookFor;
	pid_t mainPid = getpid();
    	printf("MAIN PID: %d\n",(int)mainPid); 
	if (arg != 4)
	{
		printf("Zła liczba argumentów\n");
		exit(1);
	
	}
	else
	{
		startDir = calloc(1,sizeof(argv[1]) +1);
		strcpy(startDir,argv[1]);
		lookFor = calloc(1,sizeof(argv[2]) +1);
		strcpy(lookFor,argv[2]);
		n = atoi(argv[3]);
	}
	struct dirent *dir;	
	char* curDir= calloc(1,sizeof(startDir) + 1);
	curDir = strcpy(curDir,startDir);
	pid_t waitPid;
        int status = 0;
	DIR *dp = opendir(startDir);
	pid_t child_pid;
	if(dp)
	{
		while((dir = readdir(dp)) != 0 && n > 0)
		{
			
			curDir = calloc(1,strlen(startDir) + 1);
			
			strcpy(curDir,startDir);
			
			if(strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..") == 0)
			continue;			
			if (dir->d_type == DT_DIR)
			{	
				char a = '/';
				curDir = realloc(curDir,strlen(curDir) + 2+ strlen(dir->d_name));
				strncat(curDir,&a,1);
				
				strncat(curDir,dir->d_name,strlen(dir->d_name));
				if (n != 1)
					child_pid = fork();
				if ( mainPid == getppid())
				{
				n = n-1;
				mainPid = getpid();
				free(startDir);				
				startDir = calloc(1, strlen(curDir)+1);
				strcpy(startDir,curDir);
				dp = opendir(startDir);
				}
				else
				{
					while ((waitPid = wait(&status)) > 0);
				}
							
			}
			else
			{	if( isText(dir->d_name) == 1) 
				{
					char a = '/';
					curDir = realloc(curDir,strlen(curDir) +2+ strlen(dir->d_name));
					strncat(curDir,&a,1);
					strncat(curDir,dir->d_name,strlen(dir->d_name));
					FILE *fp = fopen(curDir,"r");
					if (fp == NULL)
					{
						printf("error while opening a file: %s , in process :%d \n",curDir,(int)getpid());
						exit(1);
					}
					
					int lineNum = 1;
					char tmp[256];
					int findRes = 0;				
					while(fgets(tmp, 256, fp) != NULL) 
					{
						if( (strstr(tmp, lookFor)) != NULL) 
						{
							findRes++;
							break;
						}
						lineNum++;
					}
					if (findRes == 1)
					{
						printf("File: %s MÓJ PID: %d\n",curDir,(int)getpid() );
					}
					fclose(fp);
	
				}

			}
			
			
			free(curDir);
		}
		closedir(dp);
	}

	while ((waitPid = wait(&status)) > 0);
	
	return 0;		
}
