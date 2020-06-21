//------------------------------------------------------------------------
// File    
//     sort-main.cpp
// Description:
//     1. The program sort a sequence of N integer numbers by creating and one 
//     master thread and N sorting thread which form a pipeline.
//
//     2. An Asynchronous channel is set up between sorting thread i and i + 1, 
//     so the pipeline looks like:
//     master -- sorting thread 1 -- .. -- sorting thread i --.. -- sorting thread N
//    
//     3. The master thread read user input ending with END_OF_DATA (-1) and 
//     pass each number to the pipeline.
//     
//     4. The program output the sorted N integers in ascending order.
//------------------------------------------------------------------------

#include <iostream>
#include <string.h>
#include "ThreadClass.h"
#include "sort-thread.h"

// global data variables

SortThread *firstSortThread;        // first sorting thread 

//-----------------------------------------------------------------------
// main program
//-----------------------------------------------------------------------

int main(void)
{
     MasterThread *masterThread;

     firstSortThread = new SortThread(1, 2);    // first sorting thread 
     firstSortThread->Begin();

     masterThread = new MasterThread(1);
     masterThread->Begin();

     // wait for child threads
     masterThread->Join();
     firstSortThread->Join();

     Exit();

     return 0;
}

