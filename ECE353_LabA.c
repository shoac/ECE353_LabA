#include <stdio.h>
#include <math.h>

//input: L(line length/block size), K(number of lines per set), C(cache size)
//assuming byte addressable

main(){
  
}

int whichSet(){

}

int setIndexLength(int numberOfLines, int sizeOfLine, int cacheSize){
  //set size = K*L [number of lines * size of each line]
  //# of sets = C/(K*L) [total cache size/set size = number of sets]
  //log2(# of sets)
  return cacheSize/(numberOfLines*sizeOfLine);
}

int offsetLength(){
  //log2(L), log2 of the block size is the number of bits required for the offset
}

int tagBits(){
  //tagBits = Address length - offsetLength - setIndexLength
}

int hitWay(){
  
}

void updateOnHit(){
  
}

void updateOnMiss(){
  
}
