// ----------------------------------------------------------- 
// NAME : YunTsen Lo                         User ID: 108598056 
// DUE DATE : 05/04/2020                                       
// PROGRAM ASSIGNMENT 3                                        
// FILE NAME : thread.h        
// PROGRAM PURPOSE :
//    Head file for class QuickSortThread definition.                                         
//    This program uses multithreaded programming to
//  implement the Prefix Sum algorithm.
// ----------------------------------------------------------- 

#ifndef _THREAD_H
#define _THREAD_H

// header file
#include "ThreadClass.h"

//---------------------------------------------------------------
//  PrefixSumThread Class: 
//     each thread applies PrefixSum algorithm to compute B[stage,index]
//---------------------------------------------------------------

class PrefixSumThread : public Thread
{
     public:
          // constructor
          PrefixSumThread(vector<vector<int>>* B, int stage,int index); 

     private:
          void ThreadFunc();  // thread body
          vector<vector<int>>* _B;      //array B[*,*] of k+1 rows and n columns
                                        //to store the results of B[stage,index]
          int _stage;    //which run
          int _index;    //which element
};

#endif
