#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
	 FILE * fp1;
	 FILE * fp2;
	 FILE * fp3;
	    
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
		
    	fp1 = fopen("a.txt", "r");
    	fp2 = fopen("b.txt", "r");
    	fp3 = fopen("tmp.txt", "w"); 
	int eof1 = 0;
	int eof2 = 0;
	
	while (eof1 != 1 && eof2 !=1)
	{
	 if ( read = getline(&line, &len, fp1) != -1 && strlen(line) > 1)
	 	fprintf(fp3,"%s",line);
	 	
	 else if (strlen(line) != 1)
	 	eof1 =1;
	 
	 if ( read = getline(&line, &len, fp2) != -1 && strlen(line) > 1)
	 	fprintf(fp3,"%s",line);
	 else if (strlen(line) != 1 )
	 	eof2 = 1;

	}

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
}
