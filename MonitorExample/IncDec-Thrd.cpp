//------------------------------------------------------------------------
// Filename:
//     IncDec-Thrd.cpp
// PROGRAM DESCRIPTION
//     Class implementation file for Inc/Dec Thread class
//------------------------------------------------------------------------

#include <iostream> 
#include "ThreadClass.h"
#include "IncDec-Thrd.h"
#include "IncDec-mon.h"

#define NUM_OF_TIMES   10     // maximum number of iterations

// static data variables

static CounterMonitor Counter("CounterMonitor");

// -----------------------------------------------------------------------
// FUNCTION  Filler():
//    This function fills a strstream with spaces.
// -----------------------------------------------------------------------

strstream *Filler(int n)
{
     int  i;
     strstream *Space;

     Space = new strstream;
     for (i = 0; i < n; i++)
          (*Space) << ' ';
     (*Space) << '\0';
     return Space;
}


//------------------------------------------------------------------------
// IncrementThread::IncrementThread()
//      Constructor
//------------------------------------------------------------------------

IncDecThread::IncDecThread(int No, char ID)
               : number(No), Id(ID)
{
     ThreadName.seekp(0, ios::beg);

     if (ID == 'I')      // an increment thread
          ThreadName << "IncThread" << No << '\0';
     else
          ThreadName << "DecThread" << No << '\0';

}

//------------------------------------------------------------------------
// IncDecThread::ThreadFunc()
//      Inc/Dec thread implementation
//------------------------------------------------------------------------

void IncDecThread::ThreadFunc()
{
     Thread::ThreadFunc();
     int localCounter;
     strstream *Space;

     Space = Filler(this->number);

     for (int i = 1; i <= NUM_OF_TIMES; i++) {
          Delay();
          if (Id == 'I') {      // increase the counter
               localCounter = Counter.Increment();
               cout << Space->str() << ThreadName.str() <<
                       " increment the counter to " << localCounter << endl;
          }
          else {                // decrease the counter
               localCounter = Counter.Decrement();
               cout << Space->str() << ThreadName.str() <<
                       " decrement the counter to " << localCounter << endl;
          }
     }
     cout << Space->str() << ThreadName.str() << " Exit" << endl;
     Exit();
}


// end of IncDec-Thrd.cpp



