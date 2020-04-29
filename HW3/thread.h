// --------------------------------------------------------------//
// File : quicksort.h                                            //
//        head file for class QuickSortThread definition         //
// PROGRAM DISCRIPTION                                           //
//    This program use multithreaded programming to              //
//    implement the quicksort algorithm                          //
// ------------------------------------------------------------- //

#ifndef _THREAD_H
#define _THREAD_H

// header file
#include "ThreadClass.h"

//---------------------------------------------------------------//
//  Process Thread Class: 
//     each thread applies quicksort algorithm to divide the given
//     array and sorts them recursively
//---------------------------------------------------------------//

class PrefixSumThread : public Thread
{
     public:
          // constructor
          PrefixSumThread(vector<vector<int>>* B, int stage,int index); 

     private:
          void ThreadFunc();  // thread body
          vector<vector<int>>* _B;    // lower bound of the sub-array to be sorted
          int _stage;    // upper bound of the sub-array to be sorted
          int _index;
};

#endif
