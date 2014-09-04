#include <stdio.h>
#include <math.h>

//input: L(line length or block size), K(number of lines per set), C(cache size)
//assuming byte addressable

main(){
  unsigned int **tagArray;
  int **lruArray;
}

int whichSet(){

}

int setIndexLength(int numberOfLines, int sizeOfLine, int cacheSize){
  //set size = K*L [number of lines/set * size of each line]
  //# of sets = C/(K*L) [total cache size/set size = number of sets]
  //log2(# of sets)
  return cacheSize/(numberOfLines*sizeOfLine);
}

int offsetLength(int sizeOfLine){
  //log2(L), log2 of the block size is the number of bits required for the offset
  
}

int tagBits(int Address, int offsetLength, int setIndexLength){
  //# of tagBits = Address length - offsetLength - setIndexLength
  //address is unsigned int
  //shift right offsetLength + setIndexLength to isolate tag bits
  return 32 - setIndexLength(L,K,C) - offsetLength(L);
}

int hitWay(){
  //if hit, return cache way
  //if miss, return -1
}

void updateOnHit(){
  //validLineAge++ in lruArray
  
}

void updateOnMiss(){
  //validLineAge++ in lruArray
  //replace LRU line(greatest valid line age in lruArray) in tagArray with new tag
}
