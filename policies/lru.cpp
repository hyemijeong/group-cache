#include "lru.h"
#include <vector>
#define ll long long

LRU::LRU(ll cacheSize, ll blockSize, ll setAssociativity, int level) : 
    Cache(cacheSize, blockSize, setAssociativity, level, "LRU "){
        lastUsed = (ll *)calloc(numberOfSets * (setAssociativity), sizeof(ll));
        if(lastUsed == NULL){
            printf("Failed to allocate memory for data members of LRU (L%d) cache\n", level);
            exit(0);
        }
    }

ll LRU::getBlockToReplace(ll address, int GT){
    int index = getIndex(address);
    int min_block = index*setAssociativity;
    for(int block = index*setAssociativity; block < (index+1)*setAssociativity; block++) {
        if(lastUsed[block] == 0) // an empty slot
            return block;
        if(lastUsed[block] < lastUsed[min_block]) {
            min_block = block;
        }
    }
    return min_block; 
}

void LRU::update(ll block, int status, int time, int address){
    lastUsed[block] = time;
    time++;
}

std::vector<int> LRU::GC(int GT){
}

LRU::~LRU(){
    free(lastUsed);
}
