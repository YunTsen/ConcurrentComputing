//#include "thread-support.h"
#include "thread.h"
#include <vector>
#include <string.h>



int main(int argc, char *argv[]){
    int m,n,t;
    char buff[200];//for standard output
    vector <BabyEagleThread*> BabyEagles;
    m=atoi(argv[1]);//m pots
    n=atoi(argv[2]);//n baby eagles
    t=atoi(argv[3]);//t rounds in total


    sprintf(buff,"MAIN: There are %d baby eagles, %d feeding pots, and %d feedings.\n",n,m,t);
    write(1,buff,strlen(buff));

    //create one momEagle thread
    MomEagleThread momEagle(m,t);
    //create n babyEagle threads
    for(int i =1;i<=n;i++){
        BabyEagleThread *temBabyEagle=new BabyEagleThread(i);
        BabyEagles.push_back(temBabyEagle);
    }

    sprintf(buff,"MAIN: Game starts!!!!!\n",n,m,t);
    write(1,buff,strlen(buff));
    
    momEagle.Begin();
    for(int i =0;i<n;i++){
        BabyEagles[i]->Begin();
    }

    //wait until momEalge retires
    momEagle.Join();
    /*for(int i =0;i<n;i++){
        BabyEagles[i]->Join();
    }*/
    
    


    return 0;
}