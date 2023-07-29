#pragma once

#include "../src/cache.h"

class SRRIP : public Cache{
    private:
        int *state;

    public:
        SRRIP(long long cacheSize, long long blockSize, long long setAssociativity, int level);
        long long getBlockToReplace(long long address, int GT) override;
        void update(long long blockToReplace, int status, int time, int address) override;

        ~SRRIP();
};