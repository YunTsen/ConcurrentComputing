// -----------------------------------------------------------
// NAME : YunTsen Lo                         User ID: 108598056
// DUE DATE : 05/04/2020
// PROGRAM ASSIGNMENT 3
// FILE NAME : thread.h
// PROGRAM PURPOSE :
//    Head file for class QuickSortThread definition.
//    This program uses multithreaded programming to
//  implement the Prefix Sum algorithm.
// -----------------------------------------------------------

#ifndef _THREAD_H
#define _THREAD_H

// header file
#include "ThreadClass.h"

//---------------------------------------------------------------
//  PrefixSumThread Class:
//     each thread applies PrefixSum algorithm to compute B[stage,index]
//---------------------------------------------------------------
class EagleBaseThread : public Thread {
   public:

   protected:
    static int fullPotsNum;
    static int whoCallsMom;
    static Mutex fullPotsMutex;
    static Semaphore *potsFilled;
    static Semaphore *potsEmpty;

   private:
   virtual void ThreadFunc()=0;
    
};

class BabyEagleThread : public EagleBaseThread {
   public:
    // constructor
    BabyEagleThread(int);
    void ready_to_eat();
    void finish_eating();
    void indentation(char*);

   private:
    virtual void ThreadFunc();  //babyEagle thread body
    const int _index;   //which babyEagle
};

class MomEagleThread : public EagleBaseThread {
   public:
    //constructor
    MomEagleThread(int, int);
    void goto_sleep();
    void food_ready(int,int);
    void retire();

   private:
    virtual void ThreadFunc();  //momEagle thread body
    int _t;             //how many rounds should the mom feed her babies
    const int _m;       //how many pots are there
};

int delayTime();

#endif
