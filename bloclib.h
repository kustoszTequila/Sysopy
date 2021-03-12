

struct blockArray
{
	int blockArraySize;
	int index;
	struct pairBlock* blockArray;

};

struct pairBlock
{

	int pntArrSize;
	char** pntArr;
};


struct blockArray createBlockArray ( int size);
struct pairBlock createPairBlock(int BlockSize);
void deleteLine(struct blockArray blckArray, int index, int lineIndex);
