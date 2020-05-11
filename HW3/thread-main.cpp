// ----------------------------------------------------------- 
// NAME : YunTsen Lo                         User ID: 108598056 
// DUE DATE : 05/04/2020                                       
// PROGRAM ASSIGNMENT 3                                        
// FILE NAME : thread-main.cpp        
// PROGRAM PURPOSE :
//   This program uses multithreaded programming to             
//    implement the prefixSum algorithm
// ----------------------------------------------------------- 
#include <cmath>
#include <iostream>
#include <string.h>
#include <vector>

#include "thread.h"

using namespace std;
// ----------------------------------------------------------- 
// FUNCTION main :                       
//     Read input array, and do prefixSum using multhithreading                   
// PARAMETER USAGE : NONE          
// FUNCTION CALLED : NONE         
// ----------------------------------------------------------- 
int main() {
    int n, tem, k;
    char buf[200];  //for standard output
    vector<int> x;
    vector<vector<int>> B;
    //PrefixSumThread *prefixSumThread[8];
    vector<PrefixSumThread*> prefixSumThread;

    sprintf(buf, "Concurrent Prefix Sum Computation\n");
    write(1, buf, strlen(buf));

    //Read input array
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> tem;
        x.push_back(tem);
    }
    k = log2(n);  //How many runs
    //Prepare B[*,*] of k+1 rows and n columns.
    for (int i = 0; i < k + 1; i++) {
        B.push_back(x);
    }
    //Prepare n PrefixSumThreads
    for (int i = 0; i < n; i++) {
        prefixSumThread.push_back(new PrefixSumThread(&B, 0, 0));
    }

    sprintf(buf, "Number of input data = %d\n", n);
    write(1, buf, strlen(buf));
    sprintf(buf, "Input array:\n");
    for (int i = 0; i < n; i++) {
        sprintf(buf + strlen(buf), "%4d", x[i]);
    }
    sprintf(buf + strlen(buf), "\n");
    write(1, buf, strlen(buf));

    //start the prefixSum threads
    for (int run = 1; run < k + 1; run++) {
        sprintf(buf, "Run %d:\n", run);
        write(1, buf, strlen(buf));
        for (int index = 0; index < n; index++) {
            prefixSumThread[index] = new PrefixSumThread(&B, run, index);
            prefixSumThread[index]->Begin();
        }
        //wait for the prefixSum threads to finish
        for (int index = 0; index < n; index++) {//all elements
            prefixSumThread[index]->Join();
        }
        sprintf(buf, "Result after run %d:\n", run);
        for (int i = 0; i < n; i++) {
            sprintf(buf + strlen(buf), "%4d", B[run][i]);
        }
        sprintf(buf + strlen(buf), "\n");
        write(1, buf, strlen(buf));
    }//Prefix Sum algo ends


    sprintf(buf, "Final result after run %d:\n", k);
    for (int i = 0; i < n; i++) {
        sprintf(buf + strlen(buf), "%4d", B[k][i]);
    }
    sprintf(buf + strlen(buf), "\n");
    write(1, buf, strlen(buf));

    Exit();

    return 0;
}