// --------------------------------------------------------------//
// File : quicksort.cpp                                          //
// PROGRAM DISCRIPTION                                           //
//    Class implemention file for the QuickSortThread class      //
// ------------------------------------------------------------- //

#include <cmath>
#include <iostream>
#include <string.h>

#include "thread.h"

using namespace std;

//---------------------------------------------------------------//
//  QuickSort Class:
//     constructor
//---------------------------------------------------------------//

PrefixSumThread::PrefixSumThread(vector<vector<int>>* B, int stage, int index) : _B(B), _stage(stage), _index(index) {
    ThreadName.seekp(0, ios::beg);
    ThreadName << "Stage:"
               << _stage << ', Index:' << _index
               << '\0';
    char buf[200];
    sprintf(buf, "Stage: %d, Index: %d\n", _stage,_index);
    write(1, buf, strlen(buf));          
}

//---------------------------------------------------------------//
//  QuickSort Thread Class body:
//     each thread sort half of the a
//---------------------------------------------------------------//

void PrefixSumThread::ThreadFunc() {
    Thread::ThreadFunc();

    int n = (*_B)[0].size();
    int k = log(n);
    int gap = pow(2,_stage-1);

    

        if (_index < gap) { //B[0..gap-1] that need no calculation
            (*_B)[_stage][_index] = (*_B)[_stage - 1][_index];
        } else {            //B[gap..n] that need calculation
            (*_B)[_stage][_index] = (*_B)[_stage - 1][_index] + (*_B)[_stage - 1][_index - gap];
        }

        

        Exit();
    
}

// end of QuickSort.cpp
