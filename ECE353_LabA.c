#include <stdio.h>
#include <math.h>

//input: L(line length or block size), K(number of lines per set), C(cache size)
//assuming byte addressable

main(){
  unsigned int **tagArray; //
  int **lruArray;
  int i;
  int j;
  
  tagArray = (int**) malloc(i*sizeof(int *)); //allocates memory that can hold i*sizeof(int*) bytes for tagArray
  lruArray = (int**) malloc(i*sizeof(int *)); //allocates memory that can hold i*sizeof(int*) bytes for lruArray
  
  FILE *ifp; //declare pointer to file
  ifp = fopen("trace.txt","r"); //*ifp points to trace.txt, open for read
  while (fscanf(ifp,"%d %x",&t) != EOF){ //assuming decimal, reads decimal and hex
    //reads each access in the file(t), perform required actions here
  }
}

int whichSet(int fullAddress){    //Outputs the cache set in which the address falls.
  unsigned int a = fullAddress;   //change full address into unsigned integer
  int numTagBit = 32 - setIndexLength(L,K,C) - offsetLength(L); //calculate the number of tag bit
  a<<numTagBit;                   //get rid of tag bits, left with index|offset|0's
  a>>(numTagBit+offsetLength(L)); //get rid of offset bits and trailing 0's, leaves index
  return a;                       //return set index
  
}

int setIndexLength(int numberOfLines, int sizeOfLine, int cacheSize){
  // Outputs the number of bits in the set index field.
  //set size = K*L [number of lines/set * size of each line]
  //# of sets = C/(K*L) [total cache size/set size = number of sets]
  //log2(# of sets)
  return log2(cacheSize/(numberOfLines*sizeOfLine));
}

int offsetLength(int sizeOfLine){
  //log2(L), log2 of the block size is the number of bits required for the offset
  return log2(L);
}

int tagBits(int fullAddress){
  //Outputs the tag bits associated with the address.
  //# of tagBits = Address length - offsetLength - setIndexLength
  //address is unsigned int
  //shift right but offsetLength + setIndexLength to isolate tag bits
  unsigned int a = fullAddress;   //change full address into unsigned integer
  a>>(setIndexLength(L,K,C) + offsetLength(L));             //get rid of set index and offset bits
  return a;
}


int hitWay(int fullAddress,int numberOfLines){
  //if hit, return cache way
  //if miss, return -1
  //if hit
  for(int j = 0;j<numberOfLines;j++)
    if(tagArray[whichSet(fullAddress)][i]) == tagBits(fullAddress)) return j;
    else return -1;
}

void updateOnHit(){
  //all validLineAge++ in lruArray
  //replaces accessed line in tagArray with new tag(?) if oldTag!=newTag
}

void updateOnMiss(){
  //all validLineAge++ in lruArray
  //replace LRU line(greatest valid line age in lruArray) in tagArray with new tag
}
