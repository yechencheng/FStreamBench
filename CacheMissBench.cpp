
//
//  Created by ycc on 13-9-24.
//
/* 
Environment 
Appraoch : Hardware Performance Monitor Unit
Events : MEM_LOAD_UOPS_RETIRED.LLC_MISS  MEM_LOAD_UOPS_LLC_MISS_RETIRED.LOCAL_DRAM

Software : XCode
Optimization : O0

Software : Instrument
Sample Rate : 10ms
Function : tbb::internal::rml::private_worker::thread_routine(void*)

Profile program in Instrument, and see the event count for Function, It will be (ThreadCount - 1) * MissTime
*/

#include "Temp.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <tbb/parallel_for_each.h>
#include <vector>
#include <random>

using namespace std;

const long long CacheLen = 1024 * 1024 * 8 * 4 * 8;
const int MissTime = 1000 * 1000 * 10;
const int ThreadCount = 3;
int *Y;
int *X;
int ID[ThreadCount][MissTime];

void init()
{
    X = new int[CacheLen];
    Y = new int[CacheLen];
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<int> dis(0, CacheLen - 1);
    for(int i = 0; i < ThreadCount; i++)
        for(int j = 0; j < MissTime; j++)
            ID[i][j] = dis(gen);
}

void FlushCache()
{
    fill(Y, Y + CacheLen, 1);
    fill(X, X + CacheLen, 1);
}

int cnt = 0;

void GetCacheMissTime(int id)
{
    
	for(int i = 0; i < MissTime; i++)
		Y[ID[id][i]]++;
}


int main(int argc, char** argv)
{
    vector<int> a;
    for(int i = 0; i < ThreadCount; i++)
        a.push_back(i);
    init();
    FlushCache();
    
    cout << "BEIGN" << endl;
    tbb::parallel_for_each(a.begin(), a.end(), GetCacheMissTime);
    //GetCacheMissTime(1024 * 1024 * 16);
    cout << "END" << endl;
    cout << Y[atoi(argv[1])] << "\t" << cnt << endl;
    return 0;
}