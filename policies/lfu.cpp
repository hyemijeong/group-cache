#include "lfu.h"
#include <vector>
#define ll long long
#include<math.h>

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
     //frequency avg
    long double avg_freq = 0;
    long double avg_reuse = 0;
    long double std_freq = 0;
    long double std_reuse = 0;
    long double avg_recency =0;
    long double std_recency = 0;

    char warm = 0;

    //평균
    for(int i=0;i<cacheSize/blockSize;i++){
        avg_reuse = lastReuse[i] + avg_reuse;
        avg_freq = timesUsed[i]+ avg_freq;
        avg_recency = (GT-lastUsed[i]) + avg_recency;

        if(timesUsed[i]==0){
            warm = 1;
        } 
    }
    avg_reuse = avg_reuse/(cacheSize/blockSize);
    avg_freq = avg_freq/(cacheSize/blockSize);
    avg_recency = avg_recency/(cacheSize/blockSize);

    //표준편차
    for(int i=0;i<cacheSize/blockSize;i++){
        std_reuse = pow((lastReuse[i] - avg_reuse),2) + std_reuse;
        std_freq = pow((timesUsed[i]- avg_freq),2) + std_freq; 
        std_recency = pow(((GT-lastUsed[i])-avg_recency),2) + std_recency;
    }

    std_reuse = sqrt(std_reuse/(cacheSize/blockSize));
    std_freq = sqrt(std_freq/(cacheSize/blockSize));
    std_recency = sqrt(std_recency/(cacheSize/blockSize));


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

    //train data 추출 
    //address, avg_freq, avg_reuse, std_freq, std_reuse, warm(1:true 0:false), hit_ratio
    // printf("%d, %.2LF, %.2LF, %.2LF, %LF, %LF, %LF, %d, ",address,avg_freq,avg_reuse,avg_recency, std_freq,std_reuse,std_recency, warm);
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