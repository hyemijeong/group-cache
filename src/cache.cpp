#include "cache.h"

#define ll long long

ll readCounter=0, writeCounter=0;

// helper functions

bool isValidConfig(ll  cs, ll bs, ll sa){ // needs more analysis
    if((float)cs/(bs*sa) - (int)(cs/(bs*sa)) < 0.0001){ // ignoring precision errors
        return true;
    }
    else {
        return false;
    }
}

ll hexToDec(char hexVal[]) {
    int len = strlen(hexVal);

    // Initializing base value to 1, i.e 16^0 
    ll base = 1;
    ll decVal = 0;
    for (int i=len-1; i>=2; i--){ //2 to avoid 0x
        
        if (hexVal[i]>='0' && hexVal[i]<='9'){
            decVal += (hexVal[i] - '0')*base;
        }
        else if (hexVal[i] >= 'a' && hexVal[i] <= 'f'){
            decVal += (hexVal[i] - 'a' + 10) * base;
        }
        base *= 16; 
    }
    return decVal;
}

int log2(ll x) {
    int power = 0;
    while(x > 1) {
        x = x>>1;
        power++;
    }
    return power;
}

void incReads(){
    readCounter++;
}

void incWrites(){
    writeCounter++;
}

ll getReads(){
    return readCounter;
}

ll getWrites(){
    return writeCounter;
}

std::string Cache::getPolicy(){
    return policy;
}

// Cache class

Cache::Cache(ll cacheSize, ll blockSize, ll setAssociativity, int level, std::string policy){
    
    if(!isValidConfig(cacheSize, blockSize, setAssociativity)){
        printf("Invalid Cache configuration\n");
    }

    this->cacheSize = cacheSize;
    this->blockSize = blockSize;
    this->setAssociativity = setAssociativity;
    this->level = level;
    this->policy = policy;

    cacheBlocks = (ll*)malloc(cacheSize/blockSize * sizeof(ll));
    if(cacheBlocks == NULL){
        printf("Failed to allocate memory for L%d cache\n", this->level);
        exit(0);
    }

    AddressInBlocks = (ll*)malloc(cacheSize/blockSize * sizeof(ll));
    if(AddressInBlocks == NULL){
        printf("Failed to allocate memory for L%d cache\n", this->level);
        exit(0);
    }


    numberOfSets = cacheSize/(blockSize*setAssociativity);
    offsetSize = log2(blockSize);
    indexSize = log2(numberOfSets);

}

void Cache::incHits(){
    hits++;
}

void Cache::incMisses(){
    misses++;
}

int Cache::getLevel(){
    return level;
}

ll Cache::getTag(ll address){
    // set associate 인 경우
    // return address>>(indexSize + offsetSize);
    //fully인 경우
    return address>>offsetSize;
}

ll Cache::getIndex(ll address){
    return (address>>offsetSize) & ((1<<indexSize)-1);
}

ll Cache::getBlockPosition(ll address){
    ll index = getIndex(address);
    ll tag = getTag(address);
    ll iterator;
    for(iterator=index*setAssociativity; iterator<(index+1)*setAssociativity; iterator++){
        if(tag == cacheBlocks[iterator]){
            return iterator;
        }
    }//address에 따라 해당하는 way를 찾아서 블록을 하나하나 다 보면서 블록안에 해당하는 address가 있으면 iterator(블록의 인덱스)를 반환
    return -1;
}

void Cache::insert(ll address, ll blockToReplace){
    #ifdef DEBUG
    if(getIndex(address) != blockToReplace/setAssociativity){
        printf("ERROR: Invalid insertion: Address %x placed in block %lld", address, blockToReplace);
    }
    #endif
    cacheBlocks[blockToReplace] = getTag(address);
    AddressInBlocks[blockToReplace] = address;
}

void Cache::TimeErase(int block){
    cacheBlocks[block] = -1;
    AddressInBlocks[block] = -1;
}

ll Cache::getHits(){
    return hits;
}

ll Cache::getMisses(){
    return misses;
}

float Cache::getHitRate(){
    if(hits+misses == 0){
        return 0;
    }
    return (float)(hits)/(hits+misses);
}

Cache::~Cache(){
    free(cacheBlocks);
    free(AddressInBlocks);
}