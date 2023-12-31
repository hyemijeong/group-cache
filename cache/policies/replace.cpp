#include "replace.h"
#include "kmeans.cpp"

#define ll long long
using namespace std;


// 'REPLACE' class의 객체가 생성될 때, 초기화 리스트를 통해 상위 클래스(Cache)의 생성자 호출
// '::은 범위 지정 연산자로서 사용됨. C++에서 class나 namespace 내의 멤버에 접근할 때 사용
// REPLACE::REPLACE에서 앞에는 class REPLACE이고 뒤에는 생성자를 의미
// class 내의 멤버 함수를 정의할 때, 멤버 함수의 이름 앞에 클래스 이름과 :: 붙여서 사용
REPLACE::REPLACE(ll cacheSize, ll blockSize, ll setAssociativity, int level) :
    Cache(cacheSize, blockSize, setAssociativity, level, "REPLACE"){
        lastUsed = (ll *)calloc(numberOfSets * (setAssociativity), sizeof(ll));
        if(lastUsed == NULL){
            printf("Failed to allocate memory for data members of LRU (L%d) cache\n", level);
            exit(0);
        }

        timesUsed = (ll *)calloc(numberOfSets * (setAssociativity), sizeof(ll));
        if(timesUsed == NULL){
            printf("Failed to allocate memory for data members of LFU (L%d) cache\n", level);
            exit(0);
        }
    }

vector<pair<int,int>> runKmeans(vector<tuple<int,int,int,int>> v, int way) //index : 어떤 way에 속하는지를 알려줌
{
    srand (time(NULL));

	int total_points, total_values, K, max_iterations, has_name;

    //total_points : 데이터의 총 갯수
    //total_values : 파라미터의 총 갯수
    //k : 몇개로 클러스터링 할것인지
    //max_iterations : 최대 몇번 반복할 것인지
    //has_name : 0으로 설정하기

	//cin >> total_points >> total_values >> K >> max_iterations >> has_name; 
 
    total_points = 32;
    total_values = 4;
    K = 3;
    max_iterations = 100;
    has_name = 0;

	vector<Point> points;
	string point_name;

	for(int i = 0; i < total_points; i++)
	{
		vector<double> values;

        double value;
        double valuee;
        double valueee;
        double valueeee;

        //cin >> value; //value 입력받음 
        value = get<0>(v[i]);
        valuee = get<1>(v[i]);
        valueee= get<2>(v[i]);
        valueeee = get<3>(v[i]);

        values.push_back(value);
        values.push_back(valuee);
        values.push_back(valueee);
        values.push_back(valueeee);

		if(has_name)
		{
			//cin >> point_name;
			Point p(i, values, point_name);
			points.push_back(p);
		}
		else
		{
			Point p(i, values);
			points.push_back(p);
		}
	}

	KMeans kmeans(K, total_points, total_values, max_iterations);

    std::vector<int>::iterator iter;
	
	return kmeans.run(points,way);

}
// 1. for문 돌면서 각 block에 대한 feature를 clustering으로 보냄 - 무슨 자료구조로 담을거야?
// 2. 그럼 이제 각 block에 대해 hot, warm, cold가 정해졌겠지? 
// 3.1 현재 address가 포함된 set 내에 cold가 2개 이상이면 cold block만 학습 돌림
// 3.2 만약 cold가 한 개면 그 block이 지워짐
// 3.3 만약 cold가 없으면 어떻게 해야할까??
ll REPLACE::getBlockToReplace(ll address){
    long long victim_block = 0;
    // for(int block = 0; block < cacheSize/blockSize; block++){
    //     if(timesUsed[block] == 0 || lastUsed[block] == 0)  // an empty slot
    //         return block;
    //     v.push_back({block, lastUsed[block],timesUsed[block], address});
    // }
    v.push_back({1,5,4,5});
    v.push_back({4,5,4,8});
    v.push_back({1,7,2,6});
    v.push_back({6,5,2,8});
    v.push_back({8,0,3,2});
    v.push_back({3,9,7,2});
    v.push_back({4,5,6,0});
	v.push_back({4,45,4,16});
    v.push_back({3,7,78,6});
    v.push_back({6,5,5,8});
    v.push_back({8,2,3,3});
    v.push_back({3,9,7,79});
    v.push_back({4,20,6,34});
	v.push_back({8,20,3,3});
    v.push_back({3,14,7,79});
    v.push_back({12,70,6,34});
    v.push_back({1,5,4,5});
    v.push_back({4,5,4,8});
    v.push_back({1,7,2,6});
    v.push_back({6,5,2,8});
    v.push_back({8,0,3,2});
    v.push_back({3,9,7,2});
    v.push_back({4,5,6,0});
	v.push_back({4,45,4,16});
    v.push_back({3,7,78,6});
    v.push_back({6,5,5,8});
    v.push_back({8,2,3,3});
    v.push_back({3,9,7,79});
    v.push_back({4,20,6,34});
	v.push_back({8,20,3,3});
    v.push_back({3,14,7,79});
    v.push_back({12,70,6,34});
    


    // long long victim_block = 1;

    vector<pair<int, int>> cold = runKmeans(v, getIndex(address));
    for(auto coor : cold){
        cout << coor.first << "," << coor.second << endl;
    }
    
    return victim_block;
 }


void REPLACE::update(ll block, int status){
    lastUsed[block] = time;
    time++;

    if(status ==1){
        timesUsed[block]++;
    }
    else{
        timesUsed[block] = 0;
    }

}

// 소멸자 - 객체가 소멸될 때 필요한 정리 작업이나 메모리 해제 등의 작업 수행
REPLACE::~REPLACE(){
    free(timesUsed);
    free(lastUsed);
}