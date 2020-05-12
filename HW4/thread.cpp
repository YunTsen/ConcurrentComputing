#include "thread.h"
#include <iostream>
#include <unistd.h> 

using namespace std;

/**/
#include <string.h>
#include <unistd.h>
char buff[200]; //for standard output
/**/

int EagleBaseThread::fullPotsNum = 0;
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
    write(1,buff,strlen(buff));
    while(1){
        sleep(delayTime());//play
        ready_to_eat();
        sleep(delayTime());//eat
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
    while(round<=_t){
        goto_sleep();
        sleep(delayTime());//preparing food
        food_ready(_m,round);
        round++;
    }
    Exit();
}