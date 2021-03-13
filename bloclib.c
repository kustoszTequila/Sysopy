

#include "bloclib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pairBlock createPairBlock (int pntArrSize)
{
	if ( pntArrSize <= 0 )
	{
		return NULL;
	}
	
	else
	{
		struct pairBlock * pntArr = calloc (1, sizeof( struct pairBlock));
		
		pntArr->pntArrSize = pntArrSize;
		
		char **tmp = (char** ) calloc ( pntArrSize, sizeof( char* ));
		pntArr->pntArr = tmp;
		
		return pntArr;
	}
}

struct blockArray createBlockArray (int size)
{
	struct blockArray blckArray ;

	blckArray.blockArray= (struct pairBlock*) calloc (size, sizeof(struct pairBlock));
	blckArray.blockArraySize = size;
	blckArray.index = -1;

    return blckArray;
}
int addBlock (char* fileName, struct blockArray blckArray)
{
	struct pairBlock prBlock ;
	// pomocnicza funkcja
	blckArray.blockArray[blckArray.blockArraySize + 1 ] = prBlock;
	blckArray.blockArraySize ++;
	
	return blckArray.blockArraySize;
}
void mergeFiles(char* name1, char* name2)
{
	 FILE * fp1;
	 FILE * fp2;
	 FILE * fp3;
	    
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
		
    	fp1 = fopen(name1, "r");
    	fp2 = fopen(name2, "r");
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
void deleteLine(struct blockArray blckArray, int index, int lineIndex)
{
	if (index < 0 || lineIndex < 0 )
		return;
	else if ( index < blckArray.blockArraySize)
	{
		if (lineIndex < blckArray.blockArray[index].pntArrSize)
		{
			free(blckArray.blockArray[index].pntArr[lineIndex]);
			blckArray.blockArray[index].pntArrSize--;
		}
	}

}
void deleteBlock (struct blockArray blckArray, int index)
{
	if (index >= blckArray.blockArraySize || index < 0 )
		{
			return ;
		}
	
	free(blckArray.blockArray[index].pntArr);
	blckArray.blockArray[index].pntArrSize = 0;

}
