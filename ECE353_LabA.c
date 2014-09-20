#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <assert.h>

int **tagArray;
int **lruArray;
int rows; //# of rows of cache array
int columns;//# of columns in cache array

int logTwo(int num){//function to solve log2 of a # 
	int i = 0;
	while (num > 1){
		num = num / 2;
		i++;
	}
	return i;
} 

int twoToThePowerOf(int num){//function to for computing 2^n
	int i;
	int twos = 1;
	for (i = 0; i < num; i++)
		twos *= 2;

	return twos;
}

int offsetLength(int sizeOfLine){//returns # of bits in offset
	return logTwo(sizeOfLine);
}

int setIndexLength(int numberOfLinesPerSet, int sizeOfLine, int cacheSize){//returns # of bits in index
	int setSize = numberOfLinesPerSet * sizeOfLine;
	int numberOfSets = cacheSize / setSize;
	return logTwo(numberOfSets);
}

int tagBits(int fullAddress, int numberOfLinesPerSet, int sizeOfLine, int cacheSize){//returns  # of tag bits
	unsigned int address = fullAddress;
	return address >> (setIndexLength(numberOfLinesPerSet, sizeOfLine, cacheSize) + offsetLength(sizeOfLine)); //push out index and offset bits
}

int whichSet(int fullAddress, int numberOfLinesPerSet, int sizeOfLine, int cacheSize){//outputs cache set in which the address falls
	unsigned int address = fullAddress;
	int numberOfTagBits = 32 - setIndexLength(numberOfLinesPerSet, sizeOfLine, cacheSize) - offsetLength(sizeOfLine);
	address = address << numberOfTagBits; //push out tag bits, left with index|offset|trailing 0's
	return address >> (numberOfTagBits + offsetLength(sizeOfLine)); //push out offset bits and trailing 0's
}

int hitWay(int numberOfLinesPerSet, int set, int tag){//indicates whether the access was a hit or a muss
	int whichWay; 
	for (whichWay = 0; whichWay<numberOfLinesPerSet; whichWay++){
		if (tagArray[set][whichWay] == tag){
			return whichWay; //hit
		}
		else{
			return -1; //miss
		}
	}
}

void updateOnHit(int fullAddress, int columns, int set, int tag){
	int cacheSet = set; //sets set as the cache set in which the address falls
	int j;
	for (j = 0; j<rows; j++){//updates age required elements
		if (lruArray[cacheSet][j] != -1){//finds element in lruArray that is valid
			lruArray[cacheSet][j]++; //increase valid line age by 1
		}
	}
	lruArray[cacheSet][hitWay(fullAddress, columns, set, tag)] = 0; //sets age of valid line to 0(most recently accessed)
}

void updateOnMiss(int fullAddress, int columns, int set, int tag){
	int cacheSet = set; //sets as the cache set in which the address falls
	int j;
	int LRU = 0; //variable for oldest valid line age(least recently used line)
	int LRUj = 0; //variable for the least recently used line
	for (j = 0; j<columns; j++){
		if (lruArray[cacheSet][j]>LRU){ //finds an older valid line age in the set
			LRU = lruArray[cacheSet][j]; //replaces current oldest valid line age with older age
			LRUj = j; //line for the LRU line is updated
		}
		if (lruArray[cacheSet][j] != -1){ //if element in lruArray is valid
			lruArray[cacheSet][j]++; //increase valid line age by 1
		}
	}
	lruArray[cacheSet][LRUj] = 0; //sets age of valid line to 0(most recently accessed)
	tagArray[cacheSet][LRUj] = tag;//updates tag for LRU line in the set
}

int main(){
	//running assertions to make sure functions work
	assert(offsetLength(16) == 4); //the offset field must contain 4-bits to accomodate a 16byte cache block size
	//A 2-way with a capacity of 32kb, and a cache block contains 16bytes
	assert(setIndexLength(2, 16, 32 * 1024) == 10); //10-bits for the index
	//The address 0xFFFFCFF0 = 0d4294954992 should have a tag of all 1's, a set of 00 1111 1111, and a offset of 0
	assert(tagBits(4294954992, 2, 16, 32 * 1024) == 262143);
	assert(whichSet(4294954992, 2, 16, 32 * 1024) == 255);

	int input;

	int lineLength;
	int linesPerSet;
	long cacheSize;

	//read inputs for L,K, and C
	printf("Please enter the line length\n");
	scanf("%d", &lineLength);
	printf("Please enter the number of lines per set\n");
	scanf("%d", &linesPerSet);
	printf("Please enter the total cache size in kilobytes\n");
	scanf("%d", &cacheSize);
	cacheSize = cacheSize * 1024; //convert to bytes

	int numberOfOffsetBits = offsetLength(lineLength);
	int numberOfIndexBits = setIndexLength(linesPerSet, lineLength, cacheSize);

	rows = twoToThePowerOf(numberOfIndexBits); //calculates the # of rows needed for the cache
	columns = twoToThePowerOf(numberOfOffsetBits); //calculates the # of columns needed for the cache


	int i;

	tagArray = (int **) malloc(rows*sizeof(int)); //allocates memory that can hold rows*sizeof(int) bytes for tagArray
	for (i=0; i<rows; i++){
		*(tagArray+i) = (int **) malloc(columns*sizeof(int)); //pointer for the rows points to memory with columns
		lruArray = (int **) malloc(rows*sizeof(int)); //allocates memory that can hold rows*sizeof(int) bytes for lruArray
	}
	for (i=0; i<rows; i++){
		*(lruArray+i) = (int **) malloc(columns*sizeof(int)); //pointer for the rows points to memory with columns
	}

	double misses = 0;
	double accesses = 0;
	double missRate = 0;

	FILE *ifp;      //declare standard pointer to file
	ifp = fopen("trace.txt", "r");   //*ifp points to trace.txt, it is open for read
	if (ifp == NULL) //checks if the file doesn't exist
	{
		perror("Error in opening file");  //prints the error 
		return(-1);
	}
	while (!feof(ifp)){ //checks if the end of the file is reached
		fscanf(ifp, "%d", &input); //reads in a line of the file and stores into variable input
		printf("Address Read = %u\n", input);
		int set = whichSet(input, linesPerSet, lineLength, cacheSize);
		int tag = tagBits(input, linesPerSet, lineLength, cacheSize);
                
                //determines if the access to the address in the trace is a miss or a hit and updates the cache arrays accordingly
		if (hitWay(linesPerSet, set, tag) == -1) {
			updateOnMiss(input, columns, set, tag);
			misses++;
			accesses++;
		}
		else{
			updateOnHit(input, columns, set, tag);  
			accesses++;
		}
	}

	missRate = misses / accesses * 100;//this is for the plots
	free(tagArray); //deallocates memory previously allocated for tagArray
	free(lruArray); //deallocates memory previously allocated for lruArray
	printf("Miss Rate = %f%%\n", missRate); //outputs miss rate as a percentage

	fclose(ifp);
}
