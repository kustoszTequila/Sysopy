#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bloclib.h"
struct BlockArray* createBlockArray(int howManyPairs)
{
	struct BlockArray* bArray = calloc(1,sizeof(struct BlockArray));
	bArray->size = 0;
	bArray->blocks = calloc(howManyPairs,sizeof(struct Block*));
	
	
	return bArray;
}


struct Block* createBlock ( char* mergedFile, int size)
{
	struct Block* block = calloc(1,sizeof(struct Block));
	block->size = size;
	block->text = calloc(size,sizeof(char*));
	
	int i = 0;
	
	FILE * fp1;

	char * line = NULL;
	size_t len = 0;
	ssize_t read;

    	fp1 = fopen(mergedFile, "r");

	while (read = getline(&line, &len, fp1) != -1)
	{
		block->text[i] = malloc(1 + strlen(line));
	 	strcpy(block->text[i], line);
	 	i++;
	}
	fclose(fp1);

	return block;
}
struct Pair* createPair (char* name1, char* name2)
{
	struct Pair* pair = calloc(1,sizeof(struct Pair));
	pair->name1 = malloc( 1 + strlen(name1));
	pair->name2 = malloc( 1 + strlen(name2));
	strcpy(pair->name1,name1); 
	strcpy(pair->name2,name2); 
	pair->size = 0;
	return pair;

}
struct Sequence* createSequence(char **args,int size)
{
	if (size % 2 != 0)
		return NULL;
		
	struct Sequence* sq = calloc(1,sizeof(struct Sequence));
	
	sq->size = size;
	sq->pairs = calloc(size,sizeof(struct Pair*));
	
	for (int i = 0; i < size/2; i++)
	{
		sq->pairs[i] = createPair(args[2*i], args[2*i+1]);
	}
	return sq;
}
int addBlock (struct BlockArray* bArray,  char* mergedFile, int size)
{
	struct Block* block = createBlock(mergedFile,size);
	bArray->blocks[bArray->size] = block;
	bArray->size++;
	
	return bArray->size;
}
void mergeFiles(struct Pair* pair)
{
	 FILE * fp1;
	 FILE * fp2;
	 FILE * fp3;
	 
	int howManyLines = 0;
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	
       char * str3 = (char *) malloc(1 + strlen(pair->name1)+ strlen(pair->name2) );
       strcpy(str3, pair->name1);
       strcat(str3, pair->name2);
	
    	fp1 = fopen(pair->name1, "r");
    	fp2 = fopen(pair->name2, "r");
    	fp3 = fopen(str3, "w"); 
	int eof1 = 0;
	int eof2 = 0;
	
	while (eof1 != 1 && eof2 !=1)
	{
	 if ( read = getline(&line, &len, fp1) != -1 && strlen(line) > 1)
	 {
	 	fprintf(fp3,"%s",line);
	 	howManyLines++;
	 }
	 	
	 	
	 else if (strlen(line) != 1)
	 	eof1 =1;
	 
	 if ( read = getline(&line, &len, fp2) != -1 && strlen(line) > 1)
	 {
	 	fprintf(fp3,"%s",line);
	 	howManyLines++;
	 }
	 else if (strlen(line) != 1 )
	 	eof2 = 1;

	}

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    
    pair->margedName = (char *) malloc(1 + strlen(pair->name1)+ strlen(pair->name2) );
    strcpy(pair->margedName,str3);
    free(str3);
    pair->size = howManyLines;
}

void deleteBlock(struct BlockArray* blockArr, int index)
{

	free(blockArr->blocks[index]);
}

void deleteLine(struct Block* block, int line)
{
	free(block->text[line]);
}

int howManyLines(struct Block* block)
{
	return block->size;
}

void printFile(struct Block* block)
{

	for (int i =0; i< block->size; i++)
	{
		if (strlen(block->text[i]) > 1)
			printf("%s",block->text[i]);
	}


}
int main()
{
	char ** arg;
	arg = calloc(2,sizeof(char*));
	arg[0] = "a.txt";
	arg[1] = "b.txt";
	struct BlockArray* bArray = createBlockArray(2);
	struct Sequence* seq = createSequence(arg,2);
	mergeFiles(seq->pairs[0]);
	addBlock(bArray,seq->pairs[0]->margedName,seq->pairs[0]->size);
	/*struct Block* block = createBlock(seq->pairs[0]->margedName,seq->pairs[0]->size);
	deleteLine(block,1);*/
	printFile(bArray->blocks[0]);
	return 0;
}
