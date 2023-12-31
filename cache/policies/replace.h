#pragma once
#include <vector>
#include <tuple>
using namespace std;

#include "../src/cache.h"


class REPLACE : public Cache{  // REPLACE class가 Cache class의 기능을 상속받음
    private : // 멤버 변수
        vector<tuple<int, int, int, int>> v;
        long long time;
        long long *lastUsed;
        long long *timesUsed;

    public :
        REPLACE(long long cacheSize, long long blockSize, long long setAssociativity, int level); // 생성자
        long long getBlockToReplace(long long address) override;  // 멤버함수
        void update(long long blockToReplace, int status) override; // 멤버함수

        ~REPLACE();  // 소멸자
};