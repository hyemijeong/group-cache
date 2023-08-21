#pragma once
#include <vector>
#include "../src/cache.h"

class LFU : public Cache{
    private:
        long long *timesUsed;
        long long *lastUsed;
        // int total_count=0;
        // int count=0;
        std::vector<int> vec;

    public:
        LFU(long long cacheSize, long long blockSize, long long setAssociativity, int level);
        long long getBlockToReplace(long long address, int GT) override;
        void update(long long blockToReplace, int status, int time, int address) override;
        std::vector<int> GC(int GT) override;
        ~LFU();
};