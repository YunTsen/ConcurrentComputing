//------------------------------------------------------------------------
// Filename:
//     IncDec-mon.cpp
// PROGRAM DESCRIPTION
//     Class implementation file for Counter Monitor class
//------------------------------------------------------------------------

#include <iostream>
#include <string.h>
#include "ThreadClass.h"
#include "boat-monitor.h"
#include "thread.h"

//------------------------------------------------------------------------
// BufferMonitor::CounterMonitor()
//      Constructor
//------------------------------------------------------------------------

RiverCrossingMonitor::RiverCrossingMonitor(char* Name, int c, int m)
    : Monitor(Name, HOARE) {
    _boatLoad = 0;  // clear the counter
    _cannibals = 0;
    _missionaries = 0;
    _total = 0;

    Condition *temCon=new Condition("con");
    for (int i = 0; i < c; i++) {
        _cannibalsWait.push_back(temCon);
    }
    for (int i = 0; i < m; i++) {
        _missionarisWait.push_back(temCon);
    }
}

//------------------------------------------------------------------------
// BufferMonitor::Increment()
//      Monitor procedure Increment: increases the internal counter.
//------------------------------------------------------------------------
void RiverCrossingMonitor::showList(){
    char buff[200];
    sprintf(buff," ");
    for(int i =0;i<_queuingList.size();i++){
        if(_queuingList[i]->isCannibal()){
            sprintf(buff+strlen(buff),"c%d \n",_queuingList[i]->getIndex());
        }
        else{
            sprintf(buff+strlen(buff),"n%d \n",_queuingList[i]->getIndex());
        }
        sprintf(buff+strlen(buff),"\n");
        write(1,buff,strlen(buff));
    }
}
void RiverCrossingMonitor::passengerQueue(PassengerThread* p) {
    MonitorBegin();
    _queuingList.push_back(p);
    showList();
    if(p->isCannibal()) mySort(&_queuingList);
    _cannibals++;  //多了一隻食人怪在排隊
    _total++;
    _canPick->Signal();
    MonitorEnd();
    return;
}

void RiverCrossingMonitor::passengerOnBoard(PassengerThread *p) {
    MonitorBegin();
    if(p->isCannibal())
        _cannibalsWait[p->getIndex()]->Wait(); 
    else 
        _missionarisWait[p->getIndex()]->Wait();
    MonitorEnd();
    return;
}
void RiverCrossingMonitor::passengerOffBoard(PassengerThread *p) {
    MonitorBegin();    
    if(p->isCannibal())
        _cannibalsWait[p->getIndex()]->Wait(); 
    else 
        _missionarisWait[p->getIndex()]->Wait();
    MonitorEnd();
    return;
}
bool RiverCrossingMonitor::boatPick() {
    bool canGo=false;
    int min = 1;
    int max = _total;
    int x = rand() % (max - min + 1) + min;

    MonitorBegin();
    if(_total<3){//小於三個乘客在等
        _canPick->Wait();
    }

    if(x<=_cannibals){//第一個挑到食人族
        if(!_cannibals){
            MonitorEnd();
            return canGo;
        }
        max = _cannibals-1;
        min = 0;
        x = rand() % (max - min + 1) + min;
        listPickup(_queuingList[x],x);

        min = 1;
        max = _total;
        x = rand() % (max - min + 1) + min;
        if(x<=_cannibals){//食人族+食人族
            if(!_cannibals){
                MonitorEnd();
                return canGo;
            }
            max = _cannibals-1;
            min = 0;
            x = rand() % (max - min + 1) + min;
            listPickup(_queuingList[x],x);

            //食人族+食人族+食人族
            if(!_cannibals){
                MonitorEnd();
                return canGo;
            }
            max--;
            x = rand() % (max - min + 1) + min;
            listPickup(_queuingList[x],x);
            canGo=true;
        }
        else{//食人族+傳道士
            if(!_missionaries){
                MonitorEnd();
                return canGo;
            }
            max = _total-1;
            min = _cannibals;
            x = rand() % (max - min + 1) + min;
            listPickup(_queuingList[x],x);

            //食人族+傳道士+傳道士
            if(!_missionaries){
                MonitorEnd();
                return canGo;
            }
            max--;
            x = rand() % (max - min + 1) + min;
            listPickup(_queuingList[x],x);
            canGo=true;

        }
    }
    else{//第一個挑到傳道士
        max = _total-1;
        min = _cannibals;
        x = rand() % (max - min + 1) + min;
        listPickup(_queuingList[x],x);

        min = 1;
        max = _total;
        x = rand() % (max - min + 1) + min;
        if(x<=_cannibals){//傳道士+食人族
            if(!_cannibals){
                MonitorEnd();
                return canGo;
            }
            max = _cannibals-1;
            min = 0;
            x = rand() % (max - min + 1) + min;
            listPickup(_queuingList[x],x);

            //傳道士+食人族+傳道士
            if(!_missionaries){
                MonitorEnd();
                return canGo;
            }
            max = _total-1;
            min = _cannibals;
            x = rand() % (max - min + 1) + min;
            listPickup(_queuingList[x],x);
            canGo=true;
        }
        else{//傳道士+傳道士
            if(!_missionaries){
                MonitorEnd();
                return canGo;
            }
            max = _total-1;
            min = _cannibals;
            x = rand() % (max - min + 1) + min;
            listPickup(_queuingList[x],x);
            
            min = 1;
            max = _total;
            x = rand() % (max - min + 1) + min;
            if(x<=_cannibals){//傳道士+傳道士+食人族
                if(!_cannibals){
                    MonitorEnd();
                    return canGo;
                }
                max = _cannibals-1;
                min = 0;
                x = rand() % (max - min + 1) + min;
                listPickup(_queuingList[x],x);
                canGo=true;
            }
            else{//傳道士+傳道士+傳道士
                if(!_missionaries){
                    MonitorEnd();
                    return canGo;
                }
                max = _total-1;
                min = _cannibals;
                x = rand() % (max - min + 1) + min;
                listPickup(_queuingList[x],x);
                canGo=true;
            }
        }
    }

    if(!canGo){
        listPickupFailed();
    }
    MonitorEnd();

    return canGo;
}
void RiverCrossingMonitor::listPickup(PassengerThread *p,int x) {
    _pickupList.push_back(p);
    _queuingList.erase(_queuingList.begin()+x);
    if(p->isCannibal()){
        _cannibals--;
        mySort(&_pickupList);
    }
    else{
        _missionaries--;
    }
}

void RiverCrossingMonitor::baotOnBoard() {
    for(int i =0;i<3;i++){
        if(_pickupList[0]->isCannibal())
            _cannibalsWait[_pickupList[0]->getIndex()]->Signal();
        else
            _missionarisWait[_pickupList[0]->getIndex()]->Signal();
    }
}
void RiverCrossingMonitor::boatOffBoard() {
    for(int i =0;i<3;i++){
        if(_pickupList[0]->isCannibal())
            _cannibalsWait[_pickupList[0]->getIndex()]->Signal();
        else
            _missionarisWait[_pickupList[0]->getIndex()]->Signal();
    }
    _pickupList.clear();
}

void RiverCrossingMonitor::mySort(vector<PassengerThread*>* v){
    if((*v)[_total-1]->isCannibal()){
        swap((*v)[_total-1],(*v)[_cannibals]);
    }
}

void RiverCrossingMonitor::listPickupFailed(){
    for(int i = 0;i<_pickupList.size();i++){
            _queuingList.push_back(_pickupList[i]);
            if(_pickupList[i]->isCannibal()){
                _cannibals++;
                mySort(&_queuingList);
            }
            else{
                _missionaries++;
            }
        }
        _pickupList.clear();
}
// end of IncDec-mon.cpp
