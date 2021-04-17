#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <ctype.h>
#include <sys/wait.h>
struct ComponentArray
{
	int size; 
	struct Component** components;

};
struct Component
{
	char* name;
	int size; // number of rows
	struct Command** commands;
};

struct Command
{
	int size; 
	char* program;
	char** arguments;

};
struct Component* createComponent ( char* name)
{
	struct Component* component = calloc(1,sizeof(struct Component));
	component->size = 0;
	component->name = calloc(1,strlen(name) +1);
	strcpy(component->name,name);
	component->commands = calloc(1,sizeof(struct Command*));

	return component;
}
struct Command* createCommand(char* program)
{
	struct Command* command = calloc(1,sizeof(struct Command));
	command->program = calloc(1,strlen(program) +1);
	strcpy(command->program,program);
	command->arguments = calloc(1,sizeof(char*));
	command->size = 0;
	return command;
}
void callComponent(struct Component* component)
{
	int fd[128][2];
	pid_t pid;
	int end = component->size;

			
	for (int i = 0; i < component->size ;i++ )
	{		
		if ( i == component->size-1)
		{
			struct Command* cmd = component->commands[i];
			execvp(cmd->program,cmd->arguments);
			break;
		}	
		pipe(fd[i]);
		pid = fork();
		if (pid == 0 )
		{	close(fd[i][1]);
			dup2(fd[i][0],STDIN_FILENO);
		}
		else  
		{
			
			close(fd[i][0]);
			dup2(fd[i][1],STDOUT_FILENO);
			struct Command* cmd = component->commands[i];
			execvp(cmd->program,cmd->arguments);
			break;
			
		}
	}

}
void callCommands(struct Command** commands,int size)
{

	int fd[100][2];
	pid_t pid;	
		
	for (int i = 0; i < size ;i++ )
	{		
		if ( i == size-1)
		{
			struct Command* cmd = commands[i];
			execvp(cmd->program,cmd->arguments);
			break;
		}
		pipe(fd[i]);
		pid = fork();
		if (pid == 0 )
		{
			close(fd[i][1]);
			dup2(fd[i][0],STDIN_FILENO);
		}
		else  
		{
			close(fd[i][0]);
			dup2(fd[i][1],STDOUT_FILENO);
			struct Command* cmd = commands[i];
			execvp(cmd->program,cmd->arguments);
			break;
			
		}
	}

}
int main (int arg, char** argv)
{
	char* fileName;
	if (arg < 2 )
	{

		perror("Not enough arguments!\n");
		exit(-1);
	}
	else 
	{
		fileName = calloc(1, 1 + strlen(argv[1]));		
		strcpy(fileName,argv[1]);
	}
	FILE* fp1;
	fp1 = fopen(fileName,"r");
	size_t len = 0;
	ssize_t read;
	char * line = calloc(1,256);
	struct ComponentArray* cArray = calloc(1,sizeof(struct ComponentArray));
	cArray->size = 0;
	cArray->components = calloc(1,sizeof(struct Component*));
	while((read = getline(&line, &len, fp1) != -1))
	{
		if(strlen(line) >1)
		{

			char* pch;
			pch = strtok (line," ");
			while (pch != NULL)
			{
				if (strcmp(pch,"=") == 0)
				{// mamy polecenie
					
					int componentIndex = cArray->size -1;
					int commandIndex = 0;
					pch = strtok(NULL," ");
					cArray->components[componentIndex]->commands[commandIndex] = createCommand(pch);
					cArray->components[componentIndex ]->size++;
					cArray->components[componentIndex ]->commands = 
					realloc(cArray->components[componentIndex]->commands,
					(cArray->components[componentIndex ]->size+1) * sizeof(struct Command*));
					//
											cArray->components[componentIndex ]->commands[commandIndex]->arguments[cArray->components[componentIndex ]->
						commands[commandIndex]->size]
						 = calloc(1,strlen(pch)+1);
						strcpy(cArray->components[componentIndex ]->commands[commandIndex]
						->arguments[cArray->components[componentIndex ]->
						commands[commandIndex]->size],pch);
						cArray->components[componentIndex ]->commands[commandIndex]->size++;
						cArray->components[componentIndex ]->commands[commandIndex]->arguments = 
						realloc(cArray->components[componentIndex ]->commands[commandIndex]->arguments,
						cArray->components[componentIndex ]->commands[commandIndex]->size+1);
					while (pch != NULL)
					{	
						int commandIndex = cArray->components[componentIndex ]->size-1;
						int textIndex = cArray->components[componentIndex ]->
						commands[commandIndex]->size;					
						pch = strtok(NULL," ");
						char last = pch[strlen(pch)-1];

						if (strcmp(pch,"|") == 0)
						{
							pch = strtok(NULL," ");
							
							cArray->components[componentIndex]->commands[commandIndex+1] = createCommand(pch);
							cArray->components[componentIndex ]->size++;
							
							cArray->components[componentIndex ]->commands = 
							realloc(cArray->components[componentIndex]->commands,
							(cArray->components[componentIndex ]->size+1) * sizeof(struct Command*));
							//
						cArray->components[componentIndex ]->commands[commandIndex+1]->arguments[0]
						 = calloc(1,strlen(pch)+1);
						strcpy(cArray->components[componentIndex ]->commands[commandIndex+1]
						->arguments[0],pch);
						cArray->components[componentIndex ]->commands[commandIndex+1]->size++;
						cArray->components[componentIndex ]->commands[commandIndex+1]->arguments = 
						realloc(cArray->components[componentIndex ]->commands[commandIndex+1]->arguments,
						cArray->components[componentIndex ]->commands[commandIndex+1]->size+1);
						}
						else if (strlen(pch) == 1)
						{
						break;
							
						}
						else if (last == '\n') 
						{	
							char* newp = calloc(1,strlen(pch));
							strncpy(newp,pch,strlen(pch)-1);
							cArray->components[componentIndex ]->commands[commandIndex]->arguments[textIndex]
							 = calloc(1,strlen(newp)+1);
							strcpy(cArray->components[componentIndex ]->commands[commandIndex]
							->arguments[textIndex],newp);
							cArray->components[componentIndex ]->commands[commandIndex]->size++;
							cArray->components[componentIndex ]->commands[commandIndex]->arguments = 
							realloc(cArray->components[componentIndex ]->commands[commandIndex]->arguments,
							cArray->components[componentIndex ]->commands[commandIndex]->size+1);
							break;
						}
						else
						{
						
						cArray->components[componentIndex ]->commands[commandIndex]->arguments[textIndex]
						 = calloc(1,strlen(pch)+1);
						strcpy(cArray->components[componentIndex ]->commands[commandIndex]
						->arguments[textIndex],pch);
						cArray->components[componentIndex ]->commands[commandIndex]->size++;
						cArray->components[componentIndex ]->commands[commandIndex]->arguments = 
						realloc(cArray->components[componentIndex ]->commands[commandIndex]->arguments,
						cArray->components[componentIndex ]->commands[commandIndex]->size+1);
						}
						
					}
					for (int i = 0;  i <cArray->components[componentIndex ]->size;i++ )
					{
						int s = cArray->components[componentIndex ]->commands[i]->size;
						cArray->components[componentIndex ]->commands[i]->arguments[s] = NULL;
					}
				}
				else
				{
					cArray->components[cArray->size] = createComponent(pch);
					cArray->size++;
					cArray->components = realloc(cArray->components,(cArray->size+1) * sizeof(struct Component*));
								
				}
				pch = strtok(NULL," ");
			}
		}
		else
		{
			break;
		}
	} // koniec odczytywania 
	//wywołanie komend 
	struct Command** commands;	
	int comSize = 0;
	while((read = getline(&line, &len, fp1) != -1))
	{
		if(strlen(line) >1)
		{
			commands = calloc(1,sizeof(struct Command*));
			comSize = 0;
			char* pch;
			pch = strtok (line,"|");
			while(pch != NULL)
			{				
				for (int i = 0; i < cArray->size; i++)
				{
				
					if(strstr(pch,cArray->components[i]->name) != NULL)
					{
						
						realloc(commands, comSize + cArray->components[i]->size );
						if (commands == NULL)
						{
							printf("błąd alokacji pamięc\n");
							exit(1);
						}
						for (int j = 0; j < cArray->components[i]->size;j++)
						{
							commands[comSize] = cArray->components[i]->commands[j];
							comSize++;
						}
						break;
					}						
				}
				pch = strtok (NULL,"|");
			}
			if (comSize > 0)
			{
					pid_t pid;
					pid = fork();
					if (pid == 0)
					{
						callCommands(commands,comSize);
						break;
						exit(1);
					}	
					
					else
					{
					
					int status = 0;
					pid_t wpid;
					while ((wpid = wait(&status)) > 0);
					free(commands);					
					}
					

			}
			

		}
	
	}
	fclose(fp1);
	free(cArray);
	return 0;
}
