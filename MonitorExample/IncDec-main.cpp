//------------------------------------------------------------------------
// Filename:
//   IncDec-main.cpp
// PROGRAM DESCRIPTION
//   The main program that uses a monitor to increase/descrease a counter.
//------------------------------------------------------------------------

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "ThreadClass.h"
#include "IncDec-Thrd.h"
#include "IncDec-mon.h"

#define MAX_NUM        20   // maximum number of inc/dec threads

//------------------------------------------------------------------------
// main() function
//------------------------------------------------------------------------

int main(int argc, char *argv[])
{
     IncDecThread *thread[MAX_NUM];
     int NumberOfIncThreads = 0,    // number of increment theads
         NumberOfDecThreads = 0;    // number of decrement theads
     int NumberOfThreads;           // total number of threads
     int i;

     if (argc != 2) {              // verify user input
          cout << "Usage " << argv[0] << " #-of-Threads" << endl;
          exit(0);
     }
     else 
          NumberOfThreads = abs(atoi(argv[1]));

     srand((unsigned int) time(NULL));    // initialize random seed

     // create the increment/decrement threads in a random way
     for (i = 0; i < NumberOfThreads; i++) {      // create threads
          if (rand() % 10 <= 5) {  // create increment thread
               NumberOfIncThreads++;
               thread[i] = new IncDecThread(NumberOfIncThreads, 'I');
               thread[i]->Begin();
          }
          else {                   // create decrement thread
               NumberOfDecThreads++;
               thread[i] = new IncDecThread(NumberOfDecThreads, 'D');
               thread[i]->Begin();
          }
     }

     for (i = 0; i < NumberOfThreads; i++)   // joining
          thread[i]->Join();

     Exit();
}
