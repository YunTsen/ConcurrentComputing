// --------------------------------------------------------------//
// File : quicksort.cpp                                          //
// PROGRAM DISCRIPTION                                           //
//    Class implemention file for the QuickSortThread class      //
// ------------------------------------------------------------- //

#include "thread.h"

#include <string.h>

#include <cmath>
#include <iostream>

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
}

//---------------------------------------------------------------//
//  QuickSort Thread Class body:
//     each thread sort half of the a
//---------------------------------------------------------------//

void PrefixSumThread::ThreadFunc() {
    Thread::ThreadFunc();

    int n = (*_B)[0].size();
    int k = log(n);
    int gap = pow(2, _stage - 1);
    char buf[200];  //for standard output

    sprintf(buf, "     Thread %d Created\n", _index);
    write(1, buf, strlen(buf));

    if (_index < gap) {  //B[0..gap-1] that need no calculation
        sprintf(buf, "     Thread %d copies x[%d]\n", _index, _index);
        write(1, buf, strlen(buf));
        (*_B)[_stage][_index] = (*_B)[_stage - 1][_index];
    } else {  //B[gap..n] that need calculation
        sprintf(buf, "     Thread %d computes x[%d] + x[%d]\n", _index, _index, _index - gap);
        write(1, buf, strlen(buf));
        (*_B)[_stage][_index] = (*_B)[_stage - 1][_index] + (*_B)[_stage - 1][_index - gap];
    }

    sprintf(buf, "     Thread %d exits\n", _index);
    write(1, buf, strlen(buf));
    Exit();
}

// end of QuickSort.cpp
