// -----------------------------------------------------------
// NAME : YunTsen Lo                         User ID: 108598056
// DUE DATE : 06/22/2020
// PROGRAM ASSIGNMENT 6
// FILE NAME : thread.h
// PROGRAM PURPOSE :
//    class definition file for prime number thread and master thread
//    This program uses ThreadMentor's synchronous channels to
//    implement the sieve program
// -----------------------------------------------------------

#ifndef THREAD_H 
#define THREAD_H

#include <iostream>
#include "ThreadClass.h"

#define END_OF_DATA 1000 // end of input flag

//-------------------------------------------------------------------------
// PrimeNumberThread Class definition
//-------------------------------------------------------------------------

class PrimeNumberThread : public Thread
{
     public:
          PrimeNumberThread(int index, int threadID); // constructor and destructor
          ~PrimeNumberThread();
          SynOneToOneChannel *channel;
     private:
          void ThreadFunc();       // thread body
          int  Index;              // index of the prime number thread
          void indentation(char* buff);   // ouput format
          PrimeNumberThread *neighbor;    // next prime number thread
};

//-------------------------------------------------------------------------
// MasterThread Class
//-------------------------------------------------------------------------

class MasterThread : public Thread
{
     public:
          MasterThread(int threadID,int n);  //constructor
     private:
          void ThreadFunc();  //thread body
          int _n; //the largest integer that the Master will send to thread P2
};

#endif // THREAD_H
