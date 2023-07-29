#pragma once

#include "../src/cache.h"

class LFU : public Cache{
    private:
        long long *timesUsed;

    public:
        LFU(long long cacheSize, long long blockSize, long long setAssociativity, int level);
        long long getBlockToReplace(long long address, int GT) override;
        void update(long long blockToReplace, int status, int time, int address) override;

        ~LFU();
};