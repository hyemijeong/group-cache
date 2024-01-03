#include <stdlib.h>
#include <chrono>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <unistd.h>
#include <iostream>
#include "ioUtils.hpp" //contains I/O functions
#include "cache.h" //contains all auxillary functions
#include "../policies/lru.h"
#include "../policies/lfu.h"

using namespace std;
using namespace std::chrono;

#define ll long long

int GT;


Cache* createCacheInstance(string& policy, ll cs, ll bs, ll sa, int level){
    if(policy == "lru"){
        Cache* cache = new LRU(cs, bs, sa, level);
        return cache;
    }
    else if(policy == "lfu"){
        Cache* cache = new LFU(cs, bs, sa, level);
        return cache;
    }
}

int main(int argc, char *argv[]){
    GT =0;   
    ifstream params;
    params.open(argv[1]);
    string word;
    params >> word;
    int levels = stoi(word.c_str());

    vector<Cache*> cache(levels);

    //머신러닝 파일 불러오기 
    ifstream  ml_hitRatio("/home/hyemi/group-cache/src/msr_result.txt");
    string line;
    vector<string> ml_hitv; //머신러닝 예측 결과 벡터
    while(true) {
    	getline(ml_hitRatio, line);
        if(ml_hitRatio.eof()) break;
        ml_hitv.push_back(line);
    }
    ml_hitRatio.close();

    // for(int i =0; i<ml_hitv.size(); i++){
    //     cout << ml_hitv[i] << endl;
    // }

    int iterator = 0;
    while(iterator < levels){
        string policy;
        params >> policy;
        ll cs, bs, sa; //cacheSize, blockSize, setAssociativity
        params >> word; cs = stoll(word.c_str());
        params >> word; bs = stoll(word.c_str());
        params >> word; sa = stoll(word.c_str());
        cache[iterator++] = createCacheInstance(policy, cs, bs, sa, iterator);
    }

    #if INTERACTIVE
    initscr();
    raw();
    noecho();   
    printTraceInfoOutline();
    for(int levelItr=0; levelItr<levels; levelItr++){
        printCacheStatusOutline(cache[levelItr]);
    }
    #endif

    auto start = high_resolution_clock::now();
    while(true){
        string p;
        params >> p;
        GT=GT+1;
        
        ll address = getNextAddress();
        int hit_sum;
        
        if(address == 0) break; //reached eof
        for(int levelItr=0; levelItr<levels; levelItr++){
/////////////////////////////////끄면 lfu//////////////////////////////////////
            // if(GT%2048 == 0){ // Time GC
            //    //printf("hihi\n");
            //     for(int i=0; i<2048; i++){
            //         //이후에 GT+2048까지의 데이터가 없을 때 예외처리
            //         hit_sum = stof(ml_hitv[GT+i])+ hit_sum;
            //         cout << (GT+i) << endl;
            //         cout << (ml_hitv.size()) << endl;
            //         if(GT+i == ml_hitv.size()){
            //             break;
            //         } 
            //     }
                
            //     if(hit_sum <0){
            //         std::vector<int> vec = cache[levelItr]->GC(GT);
            //         // printf("%d\n",vec.size());
            //         for(int block : vec){
            //             cache[levelItr]->TimeErase(block);
            //         }
            //         vec.clear();
            //     }
                
            // }
/////////////////////////////////끄면 lfu//////////////////////////////////////

            ll block = cache[levelItr]->getBlockPosition(address);
            // getBlockPosition will be implemented in cache.cpp
            if(block == -1){ //cache miss
                cache[levelItr]->incMisses();
                // incMisses will be implemented in cache.cpp
                ll blockToReplace = cache[levelItr]->getBlockToReplace(address, GT);
                // getBlockToEvict will be implemented in policy.cpp
                cache[levelItr]->insert(address, blockToReplace);
                // insert will be implemented in cache.cpp
                cache[levelItr]->update(blockToReplace, 0, GT, address);

                //실시간 hit ratio 출력
                // printf("%f\n", cache[levelItr]->getHitRate());
                // update will be implemented in policy.cpp; will include updating the tree as in plru or updating the count as in lfu; 0 denotes miss
                #if INTERACTIVE
                printTraceInfo();
                printCacheStatus(cache[levelItr]);
                #endif
            }
            else{ //cache hit
                cache[levelItr]->incHits();
                // incHits will be implemented in cache.cpp
                
                cache[levelItr]->update(block, 1, GT, address);

                //실시간 hit ratio 출력
                // printf("%f\n", cache[levelItr]->getHitRate());

                //hit 되었을 때 업데이트 
                // update will be implemented in policy.cpp; 1 denotes hit
                #if INTERACTIVE
                printTraceInfo();
                printCacheStatus(cache[levelItr]);
                #endif
                break;
            }
            
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop-start);
    printf("Duration: %.3f seconds\n",duration.count() / 1000.0);

    #if INTERACTIVE
    usleep(2000000);  
    endwin();
    #endif

    printTraceInfo2();
    for(int levelItr=0; levelItr<levels; levelItr++){
        printCacheStatus2(cache[levelItr]);
        // will be implemented in cache.cpp
        delete cache[levelItr];
    }

    return 0;
}
