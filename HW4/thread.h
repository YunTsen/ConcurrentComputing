// -----------------------------------------------------------
// NAME : YunTsen Lo                         User ID: 108598056
// DUE DATE : 05/20/2020
// PROGRAM ASSIGNMENT 4
// FILE NAME : thread.h
// PROGRAM PURPOSE :
//    Head file for class EagleBaseThread &
//          BabyEagleThread & MomEagleThread definition.
//    This program uses multithreaded programming to
//    implement Hungry Eagles simulation
// -----------------------------------------------------------

#ifndef _THREAD_H
#define _THREAD_H

// header file
#include "ThreadClass.h"

//---------------------------------------------------------------
//  EagleBaseThread Class:
//     base class of BabyEagleThread and MomEagleThread
//     contains protected static member variables
//     for needed common control variables
//---------------------------------------------------------------
class EagleBaseThread : public Thread {
   public:

   protected:
    static int fullPotsNum;//number of full feeding pots
    static int whoCallsMom;//let mom knows who wake her up
    static int eatingEaglesNum;//number of eating eagles,
                               //a baby eagle should not wake up the mother 
                               //if there are baby eagles eating and the remaining pots are empty
    static Mutex fullPotsMutex;//mutex for EagleBaseThread::fullPotsNum
    static Mutex eatingEagleMutex;//mutex for EagleBaseThread::eatingEaglesNum
    static Semaphore *potsFilled;//notifies when mom fill up all feeding pots, then babies could eat
    static Semaphore *potsEmpty;//notifies when all feeding pots are empty, then mom awake

   private:
   virtual void ThreadFunc()=0;
    
};

//---------------------------------------------------------------
//  BabyEagleThread Class:
//     each baby eats from a full feeding pots and
//     wake up momEagle when all feeding pots are emtpy
//---------------------------------------------------------------
class BabyEagleThread : public EagleBaseThread {
   public:
    // constructor
    BabyEagleThread(int); //arg:index of baby eagle
    void ready_to_eat();
    void finish_eating();
    void indentation(char*); //arg:the buffer for sprintf
                             //write indentations for ouput format

   private:
    virtual void ThreadFunc();  //babyEagle thread body
    const int _index;   //which babyEagle
};

//---------------------------------------------------------------
//  MomEagleThread Class:
//     The mom sleeps until she is waken up by one babyEagle
//     Then she fills up all feeding pots and go to sleep again for t rounds
//---------------------------------------------------------------
class MomEagleThread : public EagleBaseThread {
   public:
    //constructor
    MomEagleThread(int, int);//arg:number of pots, number of feeding rounds
    void goto_sleep();
    void food_ready(int,int);//arg:how many feeding pots to be filled, which feeding round
    void retire();

   private:
    virtual void ThreadFunc();  //momEagle thread body
    int _t;             //how many rounds should the mom feed her babies
    const int _m;       //how many pots are there
};

int getDelayTime();

#endif
