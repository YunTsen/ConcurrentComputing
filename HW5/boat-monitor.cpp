//------------------------------------------------------------------------
// Filename:
//     IncDec-mon.cpp
// PROGRAM DESCRIPTION
//     Class implementation file for Counter Monitor class
//------------------------------------------------------------------------

#include "boat-monitor.h"

#include <string.h>

#include <iostream>

#include "ThreadClass.h"
#include "thread.h"

//------------------------------------------------------------------------
// BufferMonitor::CounterMonitor()
//      Constructor
//------------------------------------------------------------------------

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

    for (int i = 0; i < c; i++) {
        (*conditionName) << "C" << i << '\0';
        temCon = new Condition(conditionName->str());
        _cannibalsWait.push_back(temCon);
        temCon = new Condition(conditionName->str());
        _cannibalsWait_off.push_back(temCon);
    }
    for (int i = c; i < c+m; i++) {
        (*conditionName) << "M" << i << '\0';
        temCon = new Condition(conditionName->str());
        _missionarisWait.push_back(temCon);
        temCon = new Condition(conditionName->str());
        _missionarisWait_off.push_back(temCon);
    }
}

//------------------------------------------------------------------------
// BufferMonitor::Increment()
//      Monitor procedure Increment: increases the internal counter.
//------------------------------------------------------------------------
void RiverCrossingMonitor::showList() {
    char buff[200];
    sprintf(buff, "QL: %d\n", _total);
    for (int i = 0; i < _queuingList.size(); i++) {
        if (_queuingList[i]->isCannibal()) {
            sprintf(buff + strlen(buff), "c%d ", _queuingList[i]->getIndex());
        } else {
            sprintf(buff + strlen(buff), "m%d ", _queuingList[i]->getIndex());
        }
    }
    sprintf(buff + strlen(buff), "\n");
    write(1, buff, strlen(buff));
}

void RiverCrossingMonitor::passengerQueue(PassengerThread *p) {
    MonitorBegin();
    _queuingList.push_back(p);
    if (p->isCannibal()) {
        _cannibals++;
        _total++;
        //mySort(&_queuingList);
        swap(_queuingList[_total - 1], _queuingList[_cannibals - 1]);
    } else {
        _missionaries++;
        _total++;
    }
    //showList();
    //_canPick->Signal();
    MonitorEnd();
}

void RiverCrossingMonitor::passengerOnBoard(PassengerThread *p) {
    char buff[200];
    MonitorBegin();
    if (p->isCannibal())
        _cannibalsWait[p->getIndex()]->Wait();
    else
        _missionarisWait[p->getIndex()]->Wait();

    if (p->isCannibal()) {
        sprintf(buff, "C%d上船\n", p->getIndex());
        write(1, buff, strlen(buff));
    } else {
        sprintf(buff, "M%d上船\n", p->getIndex());
        write(1, buff, strlen(buff));
    }

    MonitorEnd();
}
void RiverCrossingMonitor::passengerOffBoard(PassengerThread *p) {
    char buff[200];
    MonitorBegin();
    if (p->isCannibal())
        _cannibalsWait_off[p->getIndex()]->Wait();
    else
        _missionarisWait_off[p->getIndex()]->Wait();

    if (p->isCannibal()) {
        sprintf(buff, "C%d下船\n", p->getIndex());
        write(1, buff, strlen(buff));
    } else {
        sprintf(buff, "M%d下船\n", p->getIndex());
        write(1, buff, strlen(buff));
    }
    MonitorEnd();
}
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

    sprintf(buff, "***** The boat is picking\n");
    write(1, buff, strlen(buff));

    if (x <= _cannibals) {  //第一個挑到食人族
        if (!_cannibals) {
            MonitorEnd();
            sprintf(buff, "沒有食人族\n");
            write(1, buff, strlen(buff));
            listPickupFailed();
            return canGo;
        }
        max = _cannibals - 1;
        min = 0;
        x = rand() % (max - min + 1) + min;
        sprintf(buff, "第一個挑到食人族%d  C%d\n", x, _queuingList[x]->getIndex());
        write(1, buff, strlen(buff));
        listPickup(_queuingList[x], x);

        min = 1;
        max = _total;
        x = rand() % (max - min + 1) + min;
        if (x <= _cannibals) {  //食人族+食人族
            if (!_cannibals) {
                sprintf(buff, "沒有食人族\n");
                write(1, buff, strlen(buff));
                listPickupFailed();
                MonitorEnd();
                return canGo;
            }
            max = _cannibals - 1;
            min = 0;
            x = rand() % (max - min + 1) + min;
            sprintf(buff, "第二個挑到食人族%d  C%d\n", x, _queuingList[x]->getIndex());
            write(1, buff, strlen(buff));
            listPickup(_queuingList[x], x);

            //食人族+食人族+食人族
            if (!_cannibals) {
                sprintf(buff, "沒有食人族\n");
                write(1, buff, strlen(buff));
                listPickupFailed();
                MonitorEnd();
                return canGo;
            }
            max--;
            x = rand() % (max - min + 1) + min;
            sprintf(buff, "第三個挑到食人族%d  C%d\n", x, _queuingList[x]->getIndex());
            write(1, buff, strlen(buff));
            listPickup(_queuingList[x], x);
            canGo = true;
        } else {  //食人族+傳道士
            if (!_missionaries) {
                sprintf(buff, "沒有傳道士\n");
                write(1, buff, strlen(buff));
                listPickupFailed();
                MonitorEnd();
                return canGo;
            }
            max = _total - 1;
            min = _cannibals;
            x = rand() % (max - min + 1) + min;
            sprintf(buff, "第二個挑到傳道士%d  M%d\n", x, _queuingList[x]->getIndex());
            write(1, buff, strlen(buff));
            listPickup(_queuingList[x], x);

            //食人族+傳道士+傳道士
            if (!_missionaries) {
                sprintf(buff, "沒有傳道士\n");
                write(1, buff, strlen(buff));
                listPickupFailed();
                MonitorEnd();
                return canGo;
            }
            max--;
            x = rand() % (max - min + 1) + min;
            sprintf(buff, "第三個挑到傳道士%d  M%d\n", x, _queuingList[x]->getIndex());
            write(1, buff, strlen(buff));
            listPickup(_queuingList[x], x);
            canGo = true;
        }
    } else {  //第一個挑到傳道士
        if (!_missionaries) {
            sprintf(buff, "沒有傳道士\n");
            write(1, buff, strlen(buff));
            listPickupFailed();
            MonitorEnd();
            return canGo;
        }
        max = _total - 1;
        min = _cannibals;
        x = rand() % (max - min + 1) + min;
        sprintf(buff, "第一個挑到傳道士%d  M%d\n", x, _queuingList[x]->getIndex());
        write(1, buff, strlen(buff));
        listPickup(_queuingList[x], x);

        min = 1;
        max = _total;
        x = rand() % (max - min + 1) + min;
        if (x <= _cannibals) {  //傳道士+食人族
            if (!_cannibals) {
                sprintf(buff, "沒有食人族\n");
                write(1, buff, strlen(buff));
                listPickupFailed();
                MonitorEnd();
                return canGo;
            }
            max = _cannibals - 1;
            min = 0;
            x = rand() % (max - min + 1) + min;
            sprintf(buff, "第二個挑到食人族%d  C%d\n", x, _queuingList[x]->getIndex());
            write(1, buff, strlen(buff));
            listPickup(_queuingList[x], x);

            //傳道士+食人族+傳道士
            if (!_missionaries) {
                sprintf(buff, "沒有傳道士\n");
                write(1, buff, strlen(buff));
                listPickupFailed();
                MonitorEnd();
                return canGo;
            }
            max = _total - 1;
            min = _cannibals;
            x = rand() % (max - min + 1) + min;
            sprintf(buff, "第三個挑到傳道士%d  M%d\n", x, _queuingList[x]->getIndex());
            write(1, buff, strlen(buff));
            listPickup(_queuingList[x], x);
            canGo = true;
        } else {  //傳道士+傳道士
            if (!_missionaries) {
                sprintf(buff, "沒有傳道士\n");
                write(1, buff, strlen(buff));
                listPickupFailed();
                MonitorEnd();
                return canGo;
            }
            max = _total - 1;
            min = _cannibals;
            x = rand() % (max - min + 1) + min;
            sprintf(buff, "第二個挑到傳道士%d  M%d\n", x, _queuingList[x]->getIndex());
            write(1, buff, strlen(buff));
            listPickup(_queuingList[x], x);

            min = 1;
            max = _total;
            x = rand() % (max - min + 1) + min;
            if (x <= _cannibals) {  //傳道士+傳道士+食人族
                if (!_cannibals) {
                    sprintf(buff, "沒有食人族\n");
                    write(1, buff, strlen(buff));
                    listPickupFailed();
                    MonitorEnd();
                    return canGo;
                }
                max = _cannibals - 1;
                min = 0;
                x = rand() % (max - min + 1) + min;
                sprintf(buff, "第三個挑到食人族%d  C%d\n", x, _queuingList[x]->getIndex());
                write(1, buff, strlen(buff));
                listPickup(_queuingList[x], x);
                canGo = true;
            } else {  //傳道士+傳道士+傳道士
                if (!_missionaries) {
                    sprintf(buff, "沒有傳道士\n");
                    write(1, buff, strlen(buff));
                    listPickupFailed();
                    MonitorEnd();
                    return canGo;
                }
                max = _total - 1;
                min = _cannibals;
                x = rand() % (max - min + 1) + min;
                sprintf(buff, "第三個挑到傳道士%d  M%d\n", x, _queuingList[x]->getIndex());
                write(1, buff, strlen(buff));
                listPickup(_queuingList[x], x);
                canGo = true;
            }
        }
    }
    MonitorEnd();

    return canGo;
}
void RiverCrossingMonitor::listPickup(PassengerThread *p, int x) {
    if (p->isCannibal()) {
        vector<PassengerThread*>::iterator it = _pickupList.begin();
        _pickupList.insert(it,p);
        _cannibals--;
        _total--;
        //swap(pick_pickupList.size()-1);
    } else {
        _pickupList.push_back(p);
        _missionaries--;
        _total--;
    }
    _queuingList.erase(_queuingList.begin() + x);
}

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
void RiverCrossingMonitor::listPickupFailed() {
    char buff[200];
    sprintf(buff, "pickList size:%d\n",_pickupList.size());
    write(1, buff, strlen(buff));
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

int RiverCrossingMonitor::getBoatLoad(){
    int b;
    MonitorBegin();
    b=_boatLoad;
    MonitorEnd();
    return b;
}
// end of IncDec-mon.cpp
