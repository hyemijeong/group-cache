#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <map>

// classes defined
class Cache;

// helper functions
bool isValidConfig(long long cs, long long bs, long long sa);
long long hexToDec(char hexVal[]);
int log2(long long x);

void incReads();
void incWrites();
long long getReads();
long long getWrites();



// cache class
class Cache{

    private:
        long long hits, misses;
        long long* cacheBlocks;
        int level;
        std::string policy;

    public:
        long long* AddressInBlocks;
        void incHits();
        void incMisses();
        int getLevel();
        std::string getPolicy();
        long long getTag(long long address);
        long long getIndex(long long address);
        long long getBlockPosition(long long address);
        void insert(long long address, long long blockToReplace);

        long long getHits();
        long long getMisses();
        float getHitRate();

        virtual long long getBlockToReplace(long long address, int GT) = 0;
        virtual void update(long long blockToReplace, int status, int time, int address) = 0;

        virtual ~Cache();

    protected:
        Cache(long long cacheSize, long long blockSize, long long setAssociativity, int level, std::string policy);
        long long cacheSize;
        long long blockSize;
        long long setAssociativity;
        long long numberOfSets;
        int offsetSize;
        int indexSize;
};