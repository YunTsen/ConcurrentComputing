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
#include "thread.h"

// global data variables
SortThread *firstSortThread;        // first sorting thread 

//-----------------------------------------------------------------------
// main program
//-----------------------------------------------------------------------

int main(int argc, char *argv[])
{    int n = (argc<2) ? 30: atoi(argv[1]);

     MasterThread *masterThread;

     masterThread = new MasterThread(1,n);
     masterThread->Begin();

     // wait for child threads
     masterThread->Join();
     firstSortThread->Join();

     Exit();

     return 0;
}

