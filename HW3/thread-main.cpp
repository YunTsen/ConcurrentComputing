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
    PrefixSumThread *prefixSumThread[8];

    sprintf(buf, "Concurrent Prefix Sum Computation\n");
    write(1, buf, strlen(buf));
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> tem;
        x.push_back(tem);
    }
    k = log2(n);
    for (int i = 0; i < k + 1; i++) {
        B.push_back(x);
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
    for(int run =1 ;run<k+1;run++){
    for (int index = 0; index < n; index++) {
        prefixSumThread[index] = new PrefixSumThread(&B, run, index);
        prefixSumThread[index]->Begin();
        prefixSumThread[index]->Join();
    }
    }

    //wait for the prefixSum thread to finish
    
    for(int stage =0;stage<k+1;stage++){
        for(int i = 0;i<n;i++){
            cout<<B[stage][i]<<"  ";
        }
        cout<<endl;
    }


    Exit();

    return 0;
}