#ifndef bloclib_h
#define bloclib_h

struct Block
{
	int size; // number of rows
	char** text;
};

struct Pair
{
	int size; // number of rows (merged)
	char* name1;
	char* name2;
	char* margedName;

};

struct Sequence
{
	int size; // how many pairs
	struct Pair** pairs; 

};

struct Block* createBlock ( char* margedFile, int size);
struct Pair* createPair (char* name1, char* name2);
struct Sequence* createSequence(char **args,int size);

void deleteBlock(struct Block** blockArr, int index);
void deleteLine(struct Block* block, int line);
int howManyLines(struct Block* block);

void printFile(struct Block* block);

#endif
