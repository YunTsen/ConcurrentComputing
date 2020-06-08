// -----------------------------------------------------------
// NAME : YunTsen Lo                         User ID: 108598056
// DUE DATE : 06/08/2020
// PROGRAM ASSIGNMENT 5
// FILE NAME : boat-monitor.h
// PROGRAM PURPOSE :
//    Head file for class RiverCrossingMonitor
// -----------------------------------------------------------

#ifndef BOAT_MONITOR_H
#define BOAT_MONITOR_H

#include <vector>
#include "ThreadClass.h"
class PassengerThread;

//---------------------------------------------------------------
//  RiverCrossingMonitor Class:
//     RiverCrossingMonitor class definition
//---------------------------------------------------------------
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

   private:
    void listPickupFailed();
    int _boatLoad;  //boat load counter
    Condition *_canPick;        //boat waits when (_tatal<3)
    vector<Condition*> _missionarisWait;    //missionaris wait here for getting onboard
    vector<Condition*> _cannibalsWait;      //cannibals wait here for getting onboard
    vector<Condition*> _missionarisWait_off;//missionaris wait here for getting offboard
    vector<Condition*> _cannibalsWait_off;  //cannibals wait here for getting offboard
    vector<PassengerThread*> _queuingList;
    vector<PassengerThread*> _pickupList;
    unsigned int _cannibals;    //number of queing cannibals
    unsigned int _missionaries; //number of queing missionaries
    unsigned int _total;        //number of queing cannibals + missionaries
};

#endif