#include <iostream>
#include <string.h>
#include <unistd.h>
#include <time.h> 

#include "thread.h"


using namespace std;

void BabyEagleThread::ready_to_eat(){
    char buff[200]; //for standard output
    sprintf(buff,"Baby eagle %d is ready to eat.\n", this->_index);
    write(1,buff,strlen(buff));
    EagleBaseThread::fullPotsMutex.Lock();
    if(EagleBaseThread::fullPotsNum>0){
        EagleBaseThread::fullPotsNum--;
         sprintf(buff,"Baby eagle %d finishes eating.\n", this->_index);
        write(1,buff,strlen(buff));
        EagleBaseThread::fullPotsMutex.Unlock();
    }
    else if (fullPotsNum==0){
        sprintf(buff,"Baby eagle %d sees all feeding pots are empty and wakes up the mother.\n", this->_index);
        write(1,buff,strlen(buff));
        //wake momEagle up
        EagleBaseThread::potsEmpty->Signal();
        EagleBaseThread::potsFilled->Wait();
        EagleBaseThread::fullPotsNum--;
        sprintf(buff,"Baby eagle %d finishes eating.\n", this->_index);
        write(1,buff,strlen(buff));
        EagleBaseThread::fullPotsMutex.Unlock();
    }
} 

void BabyEagleThread::finish_eating(){
}

void MomEagleThread::goto_sleep(){
    char buff[200]; //for standard output
    sprintf(buff,"Mother eagle takes a nap.\n");
    write(1,buff,strlen(buff));
    EagleBaseThread::potsEmpty->Wait();
}


void MomEagleThread::food_ready(int m,int round){
    
    char buff[200]; //for standard output
    EagleBaseThread::fullPotsNum=m;
    sprintf(buff,"FullPots:%d.\n", fullPotsNum);
    write(1,buff,strlen(buff));
    sprintf(buff,"Mother eagle says \"Feeding (%d)\"\n",round);
    write(1,buff,strlen(buff));
    EagleBaseThread::potsFilled->Signal();
}

int delayTime(){
    srand( time(NULL) );
    int min = 1,max=2;//1sec-3sec
    int x = rand() % (max - min + 1) + min;
    return x;
}