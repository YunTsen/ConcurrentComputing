//--------------------------------------------------------------------------
// File 
//    sort-thread.h
// PROGRAM Description:
//    class definition file for sorting thread and master thread 
//--------------------------------------------------------------------------

#ifndef _SORT_H 
#define _SORT_H 

#include <iostream>
#include "ThreadClass.h"

#define END_OF_DATA 1000 // end of input flag

//-------------------------------------------------------------------------
// SortThread Class
//-------------------------------------------------------------------------

class SortThread : public Thread
{
     public:
          SortThread(int index, int threadID); // constructor and destructor
          ~SortThread();
          SynOneToOneChannel *channel;
     private:
          void ThreadFunc();       // thread body
          int  Index;              // index of the sort thread
          void indentation(char* buff);
          SortThread *neighbor;    // next sort thread
};

//-------------------------------------------------------------------------
// MasterThread Class
//-------------------------------------------------------------------------

class MasterThread : public Thread
{
     public:
          MasterThread(int threadID,int n);
     private:
          void ThreadFunc();
          int _n;
};

#endif // _SORT_H
