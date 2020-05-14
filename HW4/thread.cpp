// ----------------------------------------------------------- 
// NAME : YunTsen Lo                         User ID: 108598056 
// DUE DATE : 05/20/2020                                       
// PROGRAM ASSIGNMENT 4                                        
// FILE NAME : thread.cpp        
// PROGRAM PURPOSE :                                           
//    Parts of class implemention file for class EagleBaseThread &
//          BabyEagleThread & MomEagleThread         
// -----------------------------------------------------------
#include "thread.h"
#include <iostream>
#include <unistd.h> 

using namespace std;

//initialize the static members in class EagleBaseThread
int EagleBaseThread::fullPotsNum = 0;
int EagleBaseThread::whoCallsMom;
int EagleBaseThread::eatingEaglesNum=0;
Mutex EagleBaseThread::eatingEagleMutex;
Mutex EagleBaseThread::fullPotsMutex;
Semaphore *EagleBaseThread::potsFilled = new Semaphore("PotsFilled",0);
Semaphore *EagleBaseThread::potsEmpty = new Semaphore("PotsEmpty",0);


// ----------------------------------------------------------- 
// FUNCTION  BabyEagleThread::BabyEagleThread :                       
//     constructor of class BabyEagleThread                          
// PARAMETER USAGE :                                           
//    int index: assign an index to this baby eagle -> _index
// FUNCTION CALLED : NONE         
// ----------------------------------------------------------- 
BabyEagleThread::BabyEagleThread(int index):_index(index){
    ThreadName.seekp(0, ios::beg);
    ThreadName << "Baby" << _index
               << '\0';
}

// ----------------------------------------------------------- 
// FUNCTION BabyEagleThread::ThreadFunc :                       
//     ThreadFunc Class body, repeats playing for a while and eating                   
// PARAMETER USAGE : NONE          
// FUNCTION CALLED :
//      void ready_to_eat()
//      void finish_eating()      
// ----------------------------------------------------------- 
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

// ----------------------------------------------------------- 
// FUNCTION  MomEagleThread::MomEagleThread :                       
//     constructor of class MomEagleThread                          
// PARAMETER USAGE :                                           
//    int m: how many pots to be filled -> _m
//    int t: how many rounds to feed    -> -t
// FUNCTION CALLED : NONE         
// -----------------------------------------------------------
MomEagleThread::MomEagleThread(int m,int t):_m(m),_t(t){
    ThreadName.seekp(0, ios::beg);
    ThreadName << "Mom " << '\0';
}

// ----------------------------------------------------------- 
// FUNCTION BabyEagleThread::ThreadFunc :                       
//     ThreadFunc Class body, repeats sleeping
//          and refilling feeding pots for _t rounds                 
// PARAMETER USAGE : NONE          
// FUNCTION CALLED :
//      void goto_sleep()
//      void food_ready(int,int)
//      void retire()
// -----------------------------------------------------------
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