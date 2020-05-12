//#include "thread-support.h"
#include "thread.h"
#include <vector>



int main(int argc, char *argv[]){
    int m,n,t;
    vector <BabyEagleThread*> BabyEagles;
    m=atoi(argv[1]);//m pots
    n=atoi(argv[2]);//n baby eagles
    t=atoi(argv[3]);//t rounds in total

    MomEagleThread momEagle(m,t);
    for(int i =0;i<n;i++){
        BabyEagleThread *temBabyEagle=new BabyEagleThread(i);
        BabyEagles.push_back(temBabyEagle);
    }
    
    momEagle.Begin();
    for(int i =0;i<n;i++){
        BabyEagles[i]->Begin();
    }
    
    


    return 0;
}