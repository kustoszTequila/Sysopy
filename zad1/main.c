#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <ctype.h>
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
	int CompNum = 0;
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
					printf("NAZWA: %s\n",pch);
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
						}
						
						else if (last == '\n') 
						{	
							strncpy(pch,pch,strlen(pch)-1);
							cArray->components[componentIndex ]->commands[commandIndex]->arguments[textIndex]
							 = calloc(1,strlen(pch)+1);
							strcpy(cArray->components[componentIndex ]->commands[commandIndex]
							->arguments[textIndex],pch);
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
					printf("wychodzimy sobie\n");
				}
				else
				{
					cArray->components[cArray->size] = createComponent(pch);
					cArray->size++;
					cArray->components = realloc(cArray->components,(cArray->size+1) * sizeof(struct Component*));
					printf("= %s\n",pch);
								
				}
				pch = strtok(NULL," ");
			}
		}
		else
		{
			break;
		}
	} // koniec odczytywania 
	for (int i = 0; i < cArray->size;i++)
	{
		for (int j = 0;j< cArray->components[i]->size; j++)
		{
			printf("komenda: %s: ",cArray->components[i]->commands[j]->program);
			for (int k = 0; k < cArray->components[i]->commands[j]->size; k++)
			{
				printf("%s ",cArray->components[i]->commands[j]->arguments[k]);
			}
			printf("\n");
		}
	}
	// wywołanie komend 
	while((read = getline(&line, &len, fp1) != -1))
	{
		if(strlen(line) >1)
		{
			char* pch;
			pch = strtok (line,"|");
			while(pch != NULL)
			{
				printf("%s \n",pch);
				pch = strtok (NULL,"|");
			}
		}
	}
	fclose(fp1);
	free(cArray);
	return 0;
}
