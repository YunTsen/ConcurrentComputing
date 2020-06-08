// -----------------------------------------------------------
// NAME : YunTsen Lo                         User ID: 108598056
// DUE DATE : 06/08/2020
// PROGRAM ASSIGNMENT 5
// FILE NAME : thread.h
// PROGRAM PURPOSE :
//    Head file for class PassengerThread &
//          CannThread & MissThread & BoatThread
//    This program uses multithreaded programming (monitor) to
//    implement River Crossing simulation
// -----------------------------------------------------------

#ifndef Thread_H
#define Thread_H

#include "ThreadClass.h"

//------------------------------------------------------------------------
// Passenger/Cann/Miss/BoatThread class definition
//------------------------------------------------------------------------
class RiverCrossingMonitor;
class PassengerThread : public Thread {
   public:
    PassengerThread(int index);     // constructor
    virtual bool isCannibal() = 0;  //test if this is a CannThread
    int getIndex();                 //return _index of this thread
    static RiverCrossingMonitor *RCM;

   protected:
    void Arrives();
    void OnBoard();
    void OffBoard();
    void indentation(char *);

   private:
    virtual void ThreadFunc() = 0;  //thread body
    int _index;
};

class CannThread : public PassengerThread {
   public:
    CannThread(int index);       // constructor
    virtual bool isCannibal();   //test if this is a CannThread

   private:
    virtual void ThreadFunc();   //thread body
};

class MissThread : public PassengerThread {
   public:
    MissThread(int index);       // constructor
    virtual bool isCannibal();   //test if this is a CannThread

   private:
    virtual void ThreadFunc();   //thread body
};

class BoatThread : public Thread {
   public:
    BoatThread(int b);  //constructor

   private:
    void ThreadFunc();  //thread body
    void BoatReady();   //boat is ready to pick
    void BoatGo();      //boat finishes selecting passenger, ready to go
    void Delay();
    void BoatDone();    //ont boat load completes
    int _b;
};

int getDelayTime();

#endif