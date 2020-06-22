// ----------------------------------------------------------- 
// NAME : YunTsen Lo                         User ID: 108598056 
// DUE DATE : 06/22/2020                                       
// PROGRAM ASSIGNMENT 6            
// FILE NAME : thread-main.cpp        
// PROGRAM PURPOSE :
//   1. The program finds out all prime numbers between 2 and n, 
//   where n is an input integer greater than 2.
//
//   2. A Synchronous channel is set up between prime numver thread i and i + 1, 
//   so the pipeline looks like:
//   master -- primeNumberThread 1 -- .. -- primeNumberThread i --.. -- primeNumberThread N
//
//   3. The program output all prime numbers between 2 and n in ascending order.
// -----------------------------------------------------------
#include <iostream>
#include <string.h>
#include "ThreadClass.h"
#include "thread.h"

// ----------------------------------------------------------- 
// FUNCTION main :                       
//     read parameter n from command lines             
//         n: to find out all prime numbers between 2 and n(default 30)
//      and run River Crossing simulation              
// PARAMETER USAGE : 
//      int argc: number of arguments
//      char *argv[]: arguments value  
// FUNCTION CALLED : NONE         
// -----------------------------------------------------------

int main(int argc, char *argv[])
{    int n = (argc<2) ? 30: atoi(argv[1]);

     MasterThread *masterThread;

     masterThread = new MasterThread(1,n);
     masterThread->Begin();

     // wait for child threads
     masterThread->Join();
     delete masterThread;

     Exit();

     return 0;
}

