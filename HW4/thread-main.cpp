//#include "thread-support.h"
#include "thread.h"
#include <vector>



int main(int argc, char *argv[]){
    int m,n,t;
    vector <BabyEagleThread*> BabyEagles;
    m=atoi(argv[1]);//m pots
    n=atoi(argv[2]);//n baby eagles
    t=atoi(argv[3]);//t rounds in total
    cout<<m<<n<<t;

    MomEagleThread momEagle(m,t);
    cout<<"!";
    for(int i =0;i<n;i++){
        BabyEagleThread *temBabyEagle=new BabyEagleThread(i);
        BabyEagles.push_back(temBabyEagle);
    }
    cout<<"?";
    
    for(int i =0;i<n;i++){
        BabyEagles[i]->Begin();
    }
    
    momEagle.Begin();


    return 0;
}