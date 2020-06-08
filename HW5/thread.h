//------------------------------------------------------------------------
// Filename:
//     IncDec-Thrd.h
// PROGRAM DESCRIPTION
//     Definition file for Increment/Decrement thread classes
//------------------------------------------------------------------------

#ifndef Thread_H
#define Thread_H

#include "ThreadClass.h"

//------------------------------------------------------------------------
// Inc/Dec thread class definition
//------------------------------------------------------------------------
class RiverCrossingMonitor;
class PassengerThread : public Thread {
   public:
    PassengerThread(int index);  // constructor
    virtual bool isCannibal() = 0;
    int getIndex();
    static RiverCrossingMonitor *RCM;

   protected:
    void Arrives();
    void OnBoard();
    void OffBoard();
    void indentation(char *);

   private:
    virtual void ThreadFunc() = 0;
    int _index;
};

class CannThread : public PassengerThread {
   public:
    CannThread(int index);  // constructor
    virtual bool isCannibal();

   private:
    virtual void ThreadFunc();
};

class MissThread : public PassengerThread {
   public:
    MissThread(int index);  // constructor
    virtual bool isCannibal();

   private:
    virtual void ThreadFunc();
};

class BoatThread : public Thread {
   public:
    BoatThread(int b);

   private:
    void ThreadFunc();
    void BoatReady();
    void BoatGo();
    void Delay();
    void BoatDone();
    int _b;
};

int getDelayTime();

#endif