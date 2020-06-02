//------------------------------------------------------------------------
// Filename:
//     IncDec-mon.h
// PROGRAM DESCRIPTION
//     Definition file for Counter Monitor class
//------------------------------------------------------------------------

#ifndef _IncDec_Mon_H 
#define _IncDec_Mon_H

#include "ThreadClass.h"

//------------------------------------------------------------------------
// BufferMonitor class definition
//------------------------------------------------------------------------

class CounterMonitor : public Monitor
{
     public:
          CounterMonitor(char* Name);   // constructor
          int Increment();              // increment the counter
          int Decrement();              // decrement the counter
     private:
          int counter;                  // internal counter
};

#endif