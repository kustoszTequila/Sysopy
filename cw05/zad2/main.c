#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <ctype.h>
#include <sys/wait.h>
#include <errno.h>
int main (int arg, char** argv)
{
	char* mail;
	char* title;
	char* text;
	if (arg == 2 )
	{
		mail = calloc(1, 1 + strlen(argv[1]));		
		strcpy(mail,argv[1]);
	}
	else if (arg == 4)
	{
		mail = calloc(1, 1 + strlen(argv[1]));		
		strcpy(mail,argv[1]);
		title = calloc(1,1 + strlen(argv[2]));
		strcpy(title,argv[2]);
		text = calloc(1,1 + strlen(argv[3]));
		strcpy(text,argv[3]);
		if (text == NULL || mail == NULL || text == NULL)
		{
			perror("Błąd alokacji danych!\n");
			exit(-1);
		}
	}
	else
	{
			perror("Zła liczba argumentów!\n");
			exit(-1);
	}
	if (strcmp(mail,"nadawca") == 0)
	{
		FILE* mail_output = popen("echo 'q' | mail | sort -k3", "r");
		int size = 1000;
		int lines = 100;
		char* tex = calloc(size, sizeof(char));
		char* res;
		for (int i =0 ; i < lines;i++)
		{
			res = fgets(tex,size,mail_output);
			if (res != NULL)
			{
				printf("%s",tex);
				if(feof(mail_output))
					break;
			}
			else
				break;	
		}		
		fclose(mail_output);
	}
	else if (strcmp(mail,"data") == 0)
	{
		FILE* mail_output = popen("echo 'q' | mail | sort -k5M -k6n -k7", "r");
		int size = 1000;
		int lines = 100;
		char* tex = calloc(size, sizeof(char));
		char* res;
		for (int i =0 ; i < lines;i++)
		{
			res = fgets(tex,size,mail_output);
			if (res != NULL)
			{
				printf("%s",tex);
				if(feof(mail_output))
					break;
			}
			else
				break;	
		}		
		fclose(mail_output);
	}
	else
	{
		char start[300] = "mail -s ";
		strcat(start,title);
		strcat(start," ");
		strcat(start,mail);
		strcat(start," ");
		FILE* mail_input = popen(start, "w");
		fputs(text,mail_input);
		fclose(mail_input);

		printf("%s",start);	
	
	}
	return 0;
	
}
