#include <string.h>

#include <cmath>
#include <iostream>
#include <vector>

#include "thread.h"

using namespace std;

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

    //start the prefixSum thread
    for (int run = 1; run < k + 1; run++) {
        sprintf(buf, "Run %d:\n", run);
        write(1, buf, strlen(buf));
        for (int index = 0; index < n; index++) {
            prefixSumThread[index] = new PrefixSumThread(&B, run, index);
            prefixSumThread[index]->Begin();
        }
        sprintf(buf, "Result after run %d:\n", run);
        for (int i = 0; i < n; i++) {
            sprintf(buf + strlen(buf), "%4d", B[run][i]);
        }
        sprintf(buf + strlen(buf), "\n");
        write(1, buf, strlen(buf));
    }

    //wait for the prefixSum threads to finish
    for (int index = 0; index < n; index++) {
        prefixSumThread[index]->Join();
    }

    sprintf(buf, "Final result after run %d:\n", k);
    for (int i = 0; i < n; i++) {
        sprintf(buf + strlen(buf), "%4d", B[k][i]);
    }
    sprintf(buf + strlen(buf), "\n");
    write(1, buf, strlen(buf));

    Exit();

    return 0;
}