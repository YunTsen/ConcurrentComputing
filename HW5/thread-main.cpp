//------------------------------------------------------------------------
// Filename:
//   IncDec-main.cpp
// PROGRAM DESCRIPTION
//   The main program that uses a monitor to increase/descrease a counter.
//------------------------------------------------------------------------

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include "ThreadClass.h"
#include "boat-monitor.h"
#include "thread.h"

//------------------------------------------------------------------------
// main() function
//------------------------------------------------------------------------

int main(int argc, char *argv[]) {
    int c = (atoi(argv[1]) == 0) ? 8 : atoi(argv[1]);  // number of increment theads
    int m = (atoi(argv[2]) == 0) ? 8 : atoi(argv[2]);                     // number of decrement theads
    int b = (atoi(argv[3]) == 0) ? 5 : atoi(argv[3]);                     // total number of threads
    vector <CannThread*> cannThreads;
    vector <MissThread*> missThreads;
    srand((unsigned int)time(NULL));  // initialize random seed

    // create the increment/decrement threads in a random way
    for (int i = 0; i < c; i++) {  // create threads
        CannThread *temCannThreads = new CannThread(i);
        cannThreads.push_back(temCannThreads);
    }
    for (int i = 0; i < m; i++) {  // create threads
        MissThread *temMissThreads = new MissThread(i);
        missThreads.push_back(temMissThreads);
    }
    BoatThread *boatThread = new BoatThread();

    boatThread->Begin();
    for(int i =0;i<c;i++){
        cannThreads[i]->Begin();
    }
    for(int i =0;i<m;i++){
        missThreads[i]->Begin();
    }
    

    Exit();
}
