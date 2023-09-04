#include "lfu.h"
#include <vector>
#define ll long long

LFU::LFU(ll cacheSize, ll blockSize, ll setAssociativity, int level) :
    Cache(cacheSize, blockSize, setAssociativity, level, "LFU"){
        timesUsed = (ll *)calloc(numberOfSets * (setAssociativity), sizeof(ll));
        if(timesUsed == NULL){
            printf("Failed to allocate memory for data members of LFU (L%d) cache\n", level);
            exit(0);
        }
        lastUsed = (ll *)calloc(numberOfSets * (setAssociativity), sizeof(ll));
        if(lastUsed == NULL){
            printf("Failed to allocate memory for data members of LRU (L%d) cache\n", level);
            exit(0);
        }
        lastReuse = (ll *)calloc(numberOfSets * (setAssociativity), sizeof(ll));
        if(lastReuse == NULL){
            printf("Failed to allocate memory for data members of LRU (L%d) cache\n", level);
            exit(0);
        }
    }

ll LFU::getBlockToReplace(ll address, int GT){
    int index = getIndex(address);
    int min_block = index*setAssociativity;
    for(int block = index*setAssociativity; block < (index+1)*setAssociativity; block++) {
        if(timesUsed[block] == 0) // an empty slot
            return block;
        if(timesUsed[block] < timesUsed[min_block]) {
            min_block = block;
        }
    }
    return min_block;
}

void LFU::update(ll block, int status, int GT, int address){
    if(status == 1){ // hit(이미 있던 page)
        lastReuse[block]= GT - lastUsed[block];
        timesUsed[block]++;
        lastUsed[block] = GT;
    }
    else{  // miss(새로 들어온 page)
        timesUsed[block] = 1;
        lastUsed[block] = GT;
        lastReuse[block] = 0;
    }
}

std::vector<int> LFU::GC(int GT){
    vec.clear();
    for(int i=0;i<cacheSize/blockSize;i++){
        int a = (GT-lastUsed[i]) - lastReuse[i];  
        // printf("%d, %d\n",GT,a/lastReuse[i]);
        if(a > 1.5*lastReuse[i]){
            
            timesUsed[i]=0;
            vec.push_back(i);            
        }
    }
    return vec;
}

LFU::~LFU(){
    free(timesUsed);
}