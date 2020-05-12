#include <iostream>
#include <string.h>
#include <unistd.h>
#include <time.h> 

#include "thread.h"


using namespace std;

void BabyEagleThread::ready_to_eat(){
    char buff[200]; //for standard output
    sprintf(buff,"Baby eagle %d is ready to eat.", this->_index);
    write(1,buff,strlen(buff));
   EagleBaseThread::fullPotsMutex.Lock();
    if(EagleBaseThread::fullPotsNum>0){
        EagleBaseThread::fullPotsNum--;
        EagleBaseThread::fullPotsMutex.Unlock();
    }
    else if (fullPotsNum==0){
        sprintf(buff,"Baby eagle %d sees all feeding pots are empty and wakes up the mother.", this->_index);
        write(1,buff,strlen(buff));
        //wake momEagle up
        EagleBaseThread::potsEmpty->Signal();
        EagleBaseThread::potsFilled->Wait();
        EagleBaseThread::fullPotsNum--;
        EagleBaseThread::fullPotsMutex.Unlock();
    }
} 

void BabyEagleThread::finish_eating(){
    char buff[200]; //for standard output
    sprintf(buff,"Baby eagle %d finishes eating.", this->_index);
    write(1,buff,strlen(buff));
}

void MomEagleThread::goto_sleep(){
    char buff[200]; //for standard output
    sprintf(buff,"Mother eagle takes a nap.");
    write(1,buff,strlen(buff));
    EagleBaseThread::potsEmpty->Wait();
}


void MomEagleThread::food_ready(int m){
    char buff[200]; //for standard output
    EagleBaseThread::fullPotsNum=m;
    sprintf(buff,"Mother eagle says \"Feeding (round)\"");
    write(1,buff,strlen(buff));
    EagleBaseThread::potsFilled->Signal();
}

int delayTime(){
    srand( time(NULL) );
    int min = 1,max=3;//1sec-3sec
    int x = rand() % (max - min + 1) + min;
    return x;
}