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
static RiverCrossingMonitor RCM("RCMonitor",8,8);

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
    RCM.passengerQueue(this);
    if(this->isCannibal()){
        sprintf(buff,"Cannibal %d arrives\n",this->getIndex());
        write(1,buff,strlen(buff));
    }
    else{
        sprintf(buff,"Missionary %d arrives\n",this->getIndex());
        write(1,buff,strlen(buff));
    }
   
}

void PassengerThread::OnBoard() {
    RCM.passengerOnBoard(this);
}

void PassengerThread::OffBoard() {
    RCM.passengerOffBoard(this);
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
        //Delay();          // take a rest
        this->Arrives();  // register to cross the river
        this->OnBoard();
        this->OffBoard();
        // other stuffs
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
        //Delay();          // take a rest
        this->Arrives();  // register to cross the river
        this->OnBoard();
        this->OffBoard();
        // other stuffs
        // come back for another river crossing
    }
    Exit();
}
BoatThread::BoatThread() {
    char buff[200];  //for standard output
    ThreadName.seekp(0, ios::beg);
    ThreadName << "Boat\0";
    sprintf(buff,"***** BOAT thread starts\n");
    write(1,buff,strlen(buff));
}

void BoatThread::ThreadFunc() {
    Thread::ThreadFunc();
    while (1) {
        //Delay();         // take a rest
        BoatReady();  // ready for the next round
        BoatGo();
        //Delay();        // row the boat
        BoatDone();  // all people are on the other side
        // come back for another river crossing
    }
    Exit();
}

void BoatThread::BoatReady(){
    char buff[200];
    sprintf(buff,"***** The boat is ready\n");
    write(1,buff,strlen(buff));
    while(!RCM.boatPick()){
        sprintf(buff,"***** The boat is picking\n");
        write(1,buff,strlen(buff));
    };
}
void BoatThread::BoatGo(){
    RCM.baotOnBoard();
}
void BoatThread::Delay(){
    sleep(getDelayTime());
}
void BoatThread::BoatDone(){
    RCM.boatOffBoard();
}

int getDelayTime() {
    srand(time(NULL));
    int min = 1, max = 2;  //1sec-2sec
    int x = rand() % (max - min + 1) + min;
    return x;
}
// end of IncDec-Thrd.cpp
