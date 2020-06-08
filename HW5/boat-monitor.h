//------------------------------------------------------------------------
// Filename:
//     IncDec-mon.h
// PROGRAM DESCRIPTION
//     Definition file for Counter Monitor class
//------------------------------------------------------------------------

#ifndef BOAT_MONITOR_H
#define BOAT_MONITOR_H

#include <vector>
#include "ThreadClass.h"
class PassengerThread;

//------------------------------------------------------------------------
// BufferMonitor class definition
//------------------------------------------------------------------------

class RiverCrossingMonitor : public Monitor {
   public:
    RiverCrossingMonitor(char* Name,int c,int m, int b);  // constructor
    void passengerQueue(PassengerThread* p);
    void passengerOnBoard(PassengerThread* p);
    void passengerOffBoard(PassengerThread* p);
    bool boatPick();
    void listPickup(PassengerThread *p,int x);
    void baotOnBoard();
    void boatOffBoard();
    void showPickList();
    void boatPickList(char *);
    int getBoatLoad();
    void endGame();

   private:
    void listPickupFailed();
    int _boatLoad;  // internal counter
    Condition *_canPick;
    vector<Condition*> _missionarisWait;
    vector<Condition*> _cannibalsWait;
    vector<Condition*> _missionarisWait_off;
    vector<Condition*> _cannibalsWait_off;
    vector<PassengerThread*> _queuingList;
    vector<PassengerThread*> _pickupList;
    unsigned int _cannibals;
    unsigned int _missionaries;
    unsigned int _total;
};

#endif