//------------------------------------------------------------------------
// Filename:
//     IncDec-mon.cpp
// PROGRAM DESCRIPTION
//     Class implementation file for Counter Monitor class
//------------------------------------------------------------------------

#include <iostream>
#include "ThreadClass.h"
#include "IncDec-mon.h"

//------------------------------------------------------------------------
// BufferMonitor::CounterMonitor()
//      Constructor
//------------------------------------------------------------------------

CounterMonitor::CounterMonitor(char* Name)
              : Monitor(Name, HOARE)
{
        counter = 0;            // clear the counter
}

//------------------------------------------------------------------------
// BufferMonitor::Increment()
//      Monitor procedure Increment: increases the internal counter.
//------------------------------------------------------------------------

int CounterMonitor::Increment()
{
     int number;

     MonitorBegin();          // lock the monitor
          counter++;          // increase the counter
          number = counter;   // save its value
     MonitorEnd();            // release the monitor
     return number;           // return the counter value
}

//------------------------------------------------------------------------
// CounterMonitor::Decremnt()
//      Monitor procedure Decrement: decreases the internal counter.
//------------------------------------------------------------------------

int CounterMonitor::Decrement()
{
     int number;

     MonitorBegin();          // lock the monitor
          counter--;          // decrease the counter
          number = counter;   // save its value
     MonitorEnd();            // release the monitor
     return number;           // return the counter value
}


// end of IncDec-mon.cpp



