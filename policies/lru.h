#pragma once

#include "../src/cache.h"

class LRU : public Cache{
    private:
        long long time;
        long long *lastUsed;
    public:
        LRU(long long cacheSize, long long blockSize, long long setAssociativity, int level);
        long long getBlockToReplace(long long address, int GT) override;
        void update(long long blockToReplace, int status, int time, int address) override;
        std::vector<int> GC(int GT) override;
        ~LRU();
};
