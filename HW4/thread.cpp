#include "thread.h"
#include <iostream>
#include <unistd.h> 

using namespace std;


int EagleBaseThread::fullPotsNum = 0;
int EagleBaseThread::whoCallsMom;
int EagleBaseThread::eatingEaglesNum=0;
Mutex EagleBaseThread::eatingEagleMutex;
Mutex EagleBaseThread::fullPotsMutex;
Semaphore *EagleBaseThread::potsFilled = new Semaphore("PotsFilled",0);
Semaphore *EagleBaseThread::potsEmpty = new Semaphore("PotsEmpty",0);


BabyEagleThread::BabyEagleThread(int index):_index(index){
    ThreadName.seekp(0, ios::beg);
    ThreadName << "Baby" << _index
               << '\0';
}

void BabyEagleThread::ThreadFunc(){
    Thread::ThreadFunc();
    while(1){
        sleep(getDelayTime());//play
        ready_to_eat();
        sleep(getDelayTime());//eat
        finish_eating();
    }
    Exit();
}


MomEagleThread::MomEagleThread(int m,int t):_m(m),_t(t){
    ThreadName.seekp(0, ios::beg);
    ThreadName << "Mom " << '\0';
}

void MomEagleThread::ThreadFunc(){
    Thread::ThreadFunc();
    static int round=0;
    while(round<_t){
        goto_sleep();
        sleep(getDelayTime());//preparing food
        food_ready(_m,round);
        round++;//next feeding round
    }
    goto_sleep();//wait until all feeding pots are empty in the last feeding round, then retire
    retire();
    exit(0);
}