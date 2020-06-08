// ----------------------------------------------------------- 
// NAME : YunTsen Lo                         User ID: 108598056 
// DUE DATE : 06/08/2020                                       
// PROGRAM ASSIGNMENT 5                                     
// FILE NAME : thread-main.cpp        
// PROGRAM PURPOSE :
//   This program uses multithreaded programming(&monitor) to             
//    implement the River Crossing simulation
// -----------------------------------------------------------

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include "ThreadClass.h"
#include "boat-monitor.h"
#include "thread.h"

// ----------------------------------------------------------- 
// FUNCTION main :                       
//     read c,m,b parameters from command lines
//         c: how many cannibals (default 8)
//         m: how many missionaries (default 8)                
//         b: how many boat loads (default 5)
//      and run River Crossing simulation              
// PARAMETER USAGE : 
//      int argc: number of arguments
//      char *argv[]: arguments value  
// FUNCTION CALLED : NONE         
// -----------------------------------------------------------

int main(int argc, char *argv[]) {
    int c = (atoi(argv[1]) == 0) ? 8 : atoi(argv[1]);  // number of increment theads
    int m = (atoi(argv[2]) == 0) ? 8 : atoi(argv[2]);                     // number of decrement theads
    int b = (atoi(argv[3]) == 0) ? 5 : atoi(argv[3]);                     // total number of threads
    vector <CannThread*> cannThreads;
    vector <MissThread*> missThreads;
    PassengerThread::RCM= new RiverCrossingMonitor("RCMonitor",c,m,b);
    
    srand((unsigned int)time(NULL));  // initialize random seed

    //create c cannibal threads
    for (int i = 0; i < c; i++) {
        CannThread *temCannThreads = new CannThread(i);
        cannThreads.push_back(temCannThreads);
    }
    //create m cannibal threads
    for (int i = 0; i < m; i++) {
        MissThread *temMissThreads = new MissThread(i);
        missThreads.push_back(temMissThreads);
    }
    //create one boat thread
    BoatThread *boatThread = new BoatThread(b);

    //run the threads
    boatThread->Begin();
    for(int i =0;i<c;i++){
        cannThreads[i]->Begin();
    }
    for(int i =0;i<m;i++){
        missThreads[i]->Begin();
    }

    //wait until boat retires
    boatThread->Join();

    return 0;
}
