//------------------------------------------------------------------------
// Filename:
//     IncDec-Thrd.cpp
// PROGRAM DESCRIPTION
//     Class implementation file for Inc/Dec Thread class
//------------------------------------------------------------------------

#include <iostream>
#include <string.h>
#include "thread.h"
#include "ThreadClass.h"
#include "boat-monitor.h"

// static data variables
RiverCrossingMonitor* PassengerThread::PassengerThread::RCM;
Mutex buffMutex;

//------------------------------------------------------------------------
// IncrementThread::IncrementThread()
//      Constructor
//------------------------------------------------------------------------

PassengerThread::PassengerThread(int index) : _index(index) {
    
}
int  PassengerThread::getIndex(){
    return _index;
}

void PassengerThread::Arrives() {
    char buff[200];  //for standard output
    PassengerThread::RCM->passengerQueue(this);
    if(this->isCannibal()){
        indentation(buff);
        sprintf(buff+strlen(buff),"Cannibal %d arrives\n",this->getIndex());
        write(1,buff,strlen(buff));
    }
    else{
        indentation(buff);
        sprintf(buff+strlen(buff),"Missionary %d arrives\n",this->getIndex());
        write(1,buff,strlen(buff));
    }
}

void PassengerThread::OnBoard() {
    PassengerThread::RCM->passengerOnBoard(this);
}

void PassengerThread::OffBoard() {
    PassengerThread::RCM->passengerOffBoard(this);
}

void PassengerThread::indentation(char* buff) {
    memset(buff, 0, strlen(buff));
    for (int i = 0; i < this->_index; i++) {
        sprintf(buff + strlen(buff), " ");
    }
}

CannThread::CannThread(int index) : PassengerThread(index) {
    char buff[200];  //for standard output
    ThreadName.seekp(0, ios::beg);
    ThreadName << "Cann" << index << '\0';
    sprintf(buff,"Cannibal %d starts\n",this->getIndex());
    write(1,buff,strlen(buff));
}

bool CannThread::isCannibal() {
    return true;
}
void CannThread::ThreadFunc() {
    Thread::ThreadFunc();
    while (1) { 
        Delay();          // take a rest
        this->Arrives();  // register to cross the river
        this->OnBoard();
        this->OffBoard();
        // come back for another river crossing
    }
    Exit();
}
//------------------------------------------------------------------------
// IncDecThread::ThreadFunc()
//      Inc/Dec thread implementation
//------------------------------------------------------------------------
MissThread::MissThread(int index) : PassengerThread(index){
    char buff[200];  //for standard output
    ThreadName.seekp(0, ios::beg);
    ThreadName << "Miss" << index << '\0';
    sprintf(buff,"Missionaries %d starts\n",this->getIndex());
    write(1,buff,strlen(buff));
}

bool MissThread::isCannibal() {
    return false;
}
void MissThread::ThreadFunc() {
    Thread::ThreadFunc();
    while (1) {
        Delay();          // take a rest
        this->Arrives();  // register to cross the river
        this->OnBoard();
        this->OffBoard();
        // come back for another river crossing
    }
    Exit();
}
BoatThread::BoatThread(int b):_b(b) {
    char buff[200];  //for standard output
    ThreadName.seekp(0, ios::beg);
    ThreadName << "Boat\0";
    sprintf(buff,"***** BOAT thread starts\n");
    write(1,buff,strlen(buff));
}

void BoatThread::ThreadFunc() {
    Thread::ThreadFunc();
    while (1) {
        Delay();         // take a rest
        BoatReady();  // ready for the next round
        BoatGo();
        Delay();        // row the boat
        BoatDone();  // all people are on the other side
        // come back for another river crossing
        if(PassengerThread::RCM->getBoatLoad()==_b){
            char buff[200];
            sprintf(buff,"MONITOR: %d crosses have been made.\nMONITOR: This river cross is closed indefinitely for renovation.\n",PassengerThread::RCM->getBoatLoad());
            write(1,buff,strlen(buff));
            break;
        }
    }
    Exit();
}

void BoatThread::BoatReady(){
    char buff[200];
    sprintf(buff,"***** The boat is ready\n");
    write(1,buff,strlen(buff));
    while(!PassengerThread::RCM->boatPick()){};
}
void BoatThread::BoatGo(){
    char buff[200];
    PassengerThread::RCM->showPickList();
    buffMutex.Lock();
    sprintf(buff,"***** Boat load (%d): Passenger list (", PassengerThread::RCM->getBoatLoad()+1);
    buffMutex.Unlock();
    PassengerThread::RCM->boatPickList(buff);
    buffMutex.Lock();
    sprintf(buff+strlen(buff),")\n");
    buffMutex.Unlock();
    write(1,buff,strlen(buff));
    PassengerThread::RCM->baotOnBoard();
}
void BoatThread::Delay(){
    sleep(getDelayTime());
}
void BoatThread::BoatDone(){
    char buff[200];
    sprintf(buff,"***** Boat load (%d): Completed\n", PassengerThread::RCM->getBoatLoad()+1);
    write(1,buff,strlen(buff));
    PassengerThread::RCM->boatOffBoard();
}

int getDelayTime() {
    srand(time(NULL));
    int min = 1, max = 2;  //1sec-2sec
    int x = rand() % (max - min + 1) + min;
    return x;
}
// end of IncDec-Thrd.cpp
