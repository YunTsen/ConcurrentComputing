// -----------------------------------------------------------
// NAME : YunTsen Lo                         User ID: 108598056
// DUE DATE : 06/08/2020
// PROGRAM ASSIGNMENT 5
// FILE NAME : boat-monitor.cpp
// PROGRAM PURPOSE :
//    Class implemention file for class RiverCrossingMonitor
// -----------------------------------------------------------

#include "boat-monitor.h"

#include <string.h>

#include <iostream>

#include "ThreadClass.h"
#include "thread.h"

 
extern Mutex buffMutex;

// ----------------------------------------------------------- 
// FUNCTION  RiverCrossingMonitor::RiverCrossingMonitor :                       
//     constructor of class RiverCrossingMonitor                          
// PARAMETER USAGE :                                           
//    int Name: assign a name to this monitor
//    int c: how many cannibals
//    int m: how many missionaries
//    int b: how many boatloads
// FUNCTION CALLED : NONE         
// -----------------------------------------------------------
RiverCrossingMonitor::RiverCrossingMonitor(char *Name, int c, int m, int b)
    : Monitor(Name, HOARE) {
    char buff[200];  //for standard output
    sprintf(buff, "Monitor starts, c:%d, m:%d, b:%d\n", c, m, b);
    write(1, buff, strlen(buff));
    _boatLoad = 0;  // clear the counter
    _cannibals = 0;
    _missionaries = 0;
    _total = 0;

    Condition *temCon;
    strstream *conditionName=new strstream;
    conditionName->seekp(0, ios::beg);
    //initialize Conditions for cannibals to wait for getting on/offboard
    for (int i = 0; i < c; i++) {
        (*conditionName) << "C" << i << '\0';
        temCon = new Condition(conditionName->str());
        _cannibalsWait.push_back(temCon);
        temCon = new Condition(conditionName->str());
        _cannibalsWait_off.push_back(temCon);
    }
    //initialize Conditions for missionaries to wait for getting on/offboard
    for (int i = c; i < c+m; i++) {
        (*conditionName) << "M" << i << '\0';
        temCon = new Condition(conditionName->str());
        _missionarisWait.push_back(temCon);
        temCon = new Condition(conditionName->str());
        _missionarisWait_off.push_back(temCon);
    }
}

// -----------------------------------------------------------
// FUNCTION  RiverCrossingMonitor::passengerQueue :
//     add passenger to queuing List
// PARAMETER USAGE :
//      PassengerThread *p: the passengerThread who wants to Queue
// FUNCTION CALLED : NONE
// -----------------------------------------------------------
void RiverCrossingMonitor::passengerQueue(PassengerThread *p) {
    MonitorBegin();
    _queuingList.push_back(p);
    if (p->isCannibal()) {
        _cannibals++;
        _total++;
        swap(_queuingList[_total - 1], _queuingList[_cannibals - 1]);//arrange _queuingList
    } else {
        _missionaries++;
        _total++;
    }
    MonitorEnd();
}
// -----------------------------------------------------------
// FUNCTION  RiverCrossingMonitor::passengerOnBoard :
//     passenger try to get onboard
// PARAMETER USAGE :
//      PassengerThread *p: the passengerThread who wants to get onboard
// FUNCTION CALLED : NONE
// -----------------------------------------------------------
void RiverCrossingMonitor::passengerOnBoard(PassengerThread *p) {
    MonitorBegin();
    if (p->isCannibal())
        _cannibalsWait[p->getIndex()]->Wait();
    else
        _missionarisWait[p->getIndex()]->Wait();
    MonitorEnd();
}
// -----------------------------------------------------------
// FUNCTION  RiverCrossingMonitor::passengerOffBoard :
//     passenger try to get offboard
// PARAMETER USAGE :
//      PassengerThread *p: the passengerThread who wants to get offboard
// FUNCTION CALLED : NONE
// -----------------------------------------------------------
void RiverCrossingMonitor::passengerOffBoard(PassengerThread *p) {
    MonitorBegin();
    if (p->isCannibal())
        _cannibalsWait_off[p->getIndex()]->Wait();
    else
        _missionarisWait_off[p->getIndex()]->Wait();
    MonitorEnd();
}
// -----------------------------------------------------------
// FUNCTION  RiverCrossingMonitor::boatPick :
//     boat tries to select passengers from _queuingList
// PARAMETER USAGE : NONE
// FUNCTION CALLED : 
//      RiverCrossingMonitor::ListPickup(PassengerThread *)
//      RiverCrossingMonitor::listPickupFailed()
// -----------------------------------------------------------
bool RiverCrossingMonitor::boatPick() {
    char buff[200];
    bool canGo = false;
    int min = 1;
    int max = _total;
    int x = rand() % (max - min + 1) + min;

    MonitorBegin();
    if (_total < 3) {  //小於三個乘客在等
        _canPick->Wait();
    }
    if (x <= _cannibals) {  //第一個挑到食人族
        if (!_cannibals) {
            MonitorEnd();
            listPickupFailed();
            return canGo;
        }
        max = _cannibals - 1;
        min = 0;
        x = rand() % (max - min + 1) + min;
        listPickup(_queuingList[x], x);

        min = 1;
        max = _total;
        x = rand() % (max - min + 1) + min;
        if (x <= _cannibals) {  //食人族+食人族
            if (!_cannibals) {
                listPickupFailed();
                MonitorEnd();
                return canGo;
            }
            max = _cannibals - 1;
            min = 0;
            x = rand() % (max - min + 1) + min;
            listPickup(_queuingList[x], x);

            //食人族+食人族+食人族
            if (!_cannibals) {
                listPickupFailed();
                MonitorEnd();
                return canGo;
            }
            max--;
            x = rand() % (max - min + 1) + min;
            listPickup(_queuingList[x], x);
            canGo = true;
        } else {  //食人族+傳道士
            if (!_missionaries) {
                listPickupFailed();
                MonitorEnd();
                return canGo;
            }
            max = _total - 1;
            min = _cannibals;
            x = rand() % (max - min + 1) + min;
            listPickup(_queuingList[x], x);

            //食人族+傳道士+傳道士
            if (!_missionaries) {
                listPickupFailed();
                MonitorEnd();
                return canGo;
            }
            max--;
            x = rand() % (max - min + 1) + min;
            listPickup(_queuingList[x], x);
            canGo = true;
        }
    } else {  //第一個挑到傳道士
        if (!_missionaries) {
            listPickupFailed();
            MonitorEnd();
            return canGo;
        }
        max = _total - 1;
        min = _cannibals;
        x = rand() % (max - min + 1) + min;
        listPickup(_queuingList[x], x);

        min = 1;
        max = _total;
        x = rand() % (max - min + 1) + min;
        if (x <= _cannibals) {  //傳道士+食人族
            if (!_cannibals) {
                listPickupFailed();
                MonitorEnd();
                return canGo;
            }
            max = _cannibals - 1;
            min = 0;
            x = rand() % (max - min + 1) + min;
            listPickup(_queuingList[x], x);

            //傳道士+食人族+傳道士
            if (!_missionaries) {
                listPickupFailed();
                MonitorEnd();
                return canGo;
            }
            max = _total - 1;
            min = _cannibals;
            x = rand() % (max - min + 1) + min;
            listPickup(_queuingList[x], x);
            canGo = true;
        } else {  //傳道士+傳道士
            if (!_missionaries) {
                listPickupFailed();
                MonitorEnd();
                return canGo;
            }
            max = _total - 1;
            min = _cannibals;
            x = rand() % (max - min + 1) + min;
            listPickup(_queuingList[x], x);

            min = 1;
            max = _total;
            x = rand() % (max - min + 1) + min;
            if (x <= _cannibals) {  //傳道士+傳道士+食人族
                if (!_cannibals) {
                    listPickupFailed();
                    MonitorEnd();
                    return canGo;
                }
                max = _cannibals - 1;
                min = 0;
                x = rand() % (max - min + 1) + min;
                listPickup(_queuingList[x], x);
                canGo = true;
            } else {  //傳道士+傳道士+傳道士
                if (!_missionaries) {
                    listPickupFailed();
                    MonitorEnd();
                    return canGo;
                }
                max = _total - 1;
                min = _cannibals;
                x = rand() % (max - min + 1) + min;
                listPickup(_queuingList[x], x);
                canGo = true;
            }
        }
    }
    MonitorEnd();

    return canGo;
}
// -----------------------------------------------------------
// FUNCTION  RiverCrossingMonitor::listPickup :
//     put the selected passenger into _pickupLick
// PARAMETER USAGE : NONE
// FUNCTION CALLED : NONE
// -----------------------------------------------------------
void RiverCrossingMonitor::listPickup(PassengerThread *p, int x) {
    if (p->isCannibal()) {
        vector<PassengerThread*>::iterator it = _pickupList.begin();
        _pickupList.insert(it,p);
        _cannibals--;
        _total--;
    } else {
        _pickupList.push_back(p);
        _missionaries--;
        _total--;
    }
    _queuingList.erase(_queuingList.begin() + x);
}
// -----------------------------------------------------------
// FUNCTION  RiverCrossingMonitor::baotOnBoard :
//     tell the selected passengers to get onboard
// PARAMETER USAGE : NONE
// FUNCTION CALLED : NONE
// -----------------------------------------------------------
void RiverCrossingMonitor::baotOnBoard() {
    MonitorBegin();
    for (int i = 0; i < 3; i++) {
        if (_pickupList[i]->isCannibal())
            _cannibalsWait[_pickupList[i]->getIndex()]->Signal();
        else
            _missionarisWait[_pickupList[i]->getIndex()]->Signal();
    }
    MonitorEnd();
}
// -----------------------------------------------------------
// FUNCTION  RiverCrossingMonitor::boatOffBoard :
//     tell the selected passengers to get offboard
// PARAMETER USAGE : NONE
// FUNCTION CALLED : NONE
// -----------------------------------------------------------
void RiverCrossingMonitor::boatOffBoard() {
    MonitorBegin();
    for (int i = 0; i < 3; i++) {
        if (_pickupList[i]->isCannibal())
            _cannibalsWait_off[_pickupList[i]->getIndex()]->Signal();
        else
            _missionarisWait_off[_pickupList[i]->getIndex()]->Signal();
    }
    _pickupList.clear();
    _boatLoad++;
    MonitorEnd();
}
// -----------------------------------------------------------
// FUNCTION  RiverCrossingMonitor::listPickupFailed :
//     select passengers failed,
//  reassign passengers in _pickupList to _queuingList
// PARAMETER USAGE : NONE
// FUNCTION CALLED : NONE
// -----------------------------------------------------------
void RiverCrossingMonitor::listPickupFailed() {
    for (int i = 0; i < _pickupList.size(); i++) {
        _queuingList.push_back(_pickupList[i]);
        if (_pickupList[i]->isCannibal()) {
            _cannibals++;
            _total++;
            swap(_queuingList[_total - 1], _queuingList[_cannibals - 1]);
        } else {
            _missionaries++;
            _total++;
            swap(_queuingList[_total - 1], _queuingList[_cannibals - 1]);
        }
    }
    _pickupList.clear();
}
// -----------------------------------------------------------
// FUNCTION  RiverCrossingMonitor::getBoatLoad :
//     return current _boatLoad
// PARAMETER USAGE : NONE
// FUNCTION CALLED : NONE
// -----------------------------------------------------------
int RiverCrossingMonitor::getBoatLoad(){
    int b;
    MonitorBegin();
    b=_boatLoad;
    MonitorEnd();
    return b;
}
// -----------------------------------------------------------
// FUNCTION  RiverCrossingMonitor::showPickList :
//     show selected passengers
// PARAMETER USAGE : NONE
// FUNCTION CALLED : NONE
// -----------------------------------------------------------
void RiverCrossingMonitor::showPickList(){
    char buff[200];
    if(_pickupList[2]->isCannibal()){
        sprintf(buff, "MONITOR(%d): three cannibals (%d, %d, %d) are selected\n",
            this->_boatLoad,_pickupList[0]->getIndex(),_pickupList[1]->getIndex(),_pickupList[2]->getIndex());
        write(1, buff, strlen(buff));
    }
    else if(_pickupList[0]->isCannibal()){
        sprintf(buff, "MONITOR(%d): one cannibal (%d) and two missionaries (%d, %d) are selected\n",
            this->_boatLoad,_pickupList[0]->getIndex(),_pickupList[1]->getIndex(),_pickupList[2]->getIndex());
        write(1, buff, strlen(buff));
    }
    else{
        sprintf(buff, "MONITOR(%d): three missionaries (%d, %d, %d) are selected\n",
            this->_boatLoad,_pickupList[0]->getIndex(),_pickupList[1]->getIndex(),_pickupList[2]->getIndex());
        write(1, buff, strlen(buff));
    }
}
// -----------------------------------------------------------
// FUNCTION  RiverCrossingMonitor::showPickList :
//     show selected passengers(stdout for boat)
// PARAMETER USAGE : NONE
// FUNCTION CALLED : NONE
// -----------------------------------------------------------
void RiverCrossingMonitor::boatPickList(char *buff){
    for(int i=0;i<3;i++){
        if(_pickupList[i]->isCannibal()){
            sprintf(buff+strlen(buff),"c%d", _pickupList[i]->getIndex());
        }
        else{
            sprintf(buff+strlen(buff),"m%d", _pickupList[i]->getIndex());
        }
        if(i==2) continue;
        sprintf(buff+strlen(buff)," ,", _pickupList[i]->getIndex());
    }
}
// end of IncDec-mon.cpp
