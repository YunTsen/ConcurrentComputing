// -----------------------------------------------------------
// NAME : YunTsen Lo                         User ID: 108598056
// DUE DATE : 06/08/2020
// PROGRAM ASSIGNMENT 5
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
//    Class implemention file for class PassengerThread &
//          CannThread & MissThread & BoatThread
// -----------------------------------------------------------

#include <iostream>
#include <string.h>
#include "thread.h"
#include "ThreadClass.h"
#include "boat-monitor.h"

// static data variable
RiverCrossingMonitor* PassengerThread::PassengerThread::RCM;

// ----------------------------------------------------------- 
// FUNCTION  PassengerThread::PassengerThread :                       
//     constructor of class PassengerThread                          
// PARAMETER USAGE :                                           
//    int index: assign an index to this Passenger
// FUNCTION CALLED : NONE         
// ----------------------------------------------------------- 
PassengerThread::PassengerThread(int index) : _index(index) {}
// ----------------------------------------------------------- 
// FUNCTION  PassengerThread::getIndex :                       
//     return _index of this PassengerThread                          
// PARAMETER USAGE : NONE                                 
// FUNCTION CALLED : NONE         
// -----------------------------------------------------------
int  PassengerThread::getIndex(){
    return _index;
}
// ----------------------------------------------------------- 
// FUNCTION  PassengerThread::Arrives :                       
//     Passenger arrives and waits for getting onboard                          
// PARAMETER USAGE : NONE                                 
// FUNCTION CALLED :
//     RiverCrossingMonitor::passengerQueue(PassengerThread* p);          
// -----------------------------------------------------------
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
// ----------------------------------------------------------- 
// FUNCTION  PassengerThread::Arrives :                       
//     Passenger being selected, and get onboard                          
// PARAMETER USAGE : NONE                                 
// FUNCTION CALLED :
//     RiverCrossingMonitor::passengerOnBoard(PassengerThread* p);          
// -----------------------------------------------------------
void PassengerThread::OnBoard() {
    PassengerThread::RCM->passengerOnBoard(this);
}
// ----------------------------------------------------------- 
// FUNCTION  PassengerThread::Arrives :                       
//     The boat arrives, and passenger could get offboard                          
// PARAMETER USAGE : NONE                                 
// FUNCTION CALLED :
//     RiverCrossingMonitor::passengerOffBoard(PassengerThread* p);          
// -----------------------------------------------------------
void PassengerThread::OffBoard() {
    PassengerThread::RCM->passengerOffBoard(this);
}
// -----------------------------------------------------------
// FUNCTION  BabyEagleThread::indentation :
//     handles the needed indentaion for PassengerThread
// PARAMETER USAGE :
//      char* buff: the buffer for standard output
// FUNCTION CALLED : NONE
// -----------------------------------------------------------
void PassengerThread::indentation(char* buff) {
    memset(buff, 0, strlen(buff));
    for (int i = 0; i < this->_index; i++) {
        sprintf(buff + strlen(buff), " ");
    }
}
// ----------------------------------------------------------- 
// FUNCTION  CannThread::CannThread :                       
//     constructor of class CannThread                          
// PARAMETER USAGE :                                           
//    int index: assign an index to this CannThread
// FUNCTION CALLED : NONE         
// -----------------------------------------------------------
CannThread::CannThread(int index) : PassengerThread(index) {
    char buff[200];  //for standard output
    ThreadName.seekp(0, ios::beg);
    ThreadName << "Cann" << index << '\0';
    sprintf(buff,"Cannibal %d starts\n",this->getIndex());
    write(1,buff,strlen(buff));
}
// ----------------------------------------------------------- 
// FUNCTION  CannThread::isCannibal :                       
//     check if this thread is a CannThread, returns 1 if true                        
// PARAMETER USAGE : NONE
// FUNCTION CALLED : NONE         
// -----------------------------------------------------------
bool CannThread::isCannibal() {
    return true;
}
// ----------------------------------------------------------- 
// FUNCTION CannThread::ThreadFunc :                       
//     ThreadFunc Class body, repeats getting onboard and offboard                   
// PARAMETER USAGE : NONE          
// FUNCTION CALLED :
//      void PassengerThread::Arrives()
//      void PassengerThread::OnBoard()      
//      void PassengerThread::OffBoard()    
//      int getDelayTime() 
// -----------------------------------------------------------
void CannThread::ThreadFunc() {
    Thread::ThreadFunc();
    while (1) { 
        sleep(getDelayTime());          // take a rest
        this->Arrives();  // register to cross the river
        this->OnBoard();
        this->OffBoard();
        // come back for another river crossing
    }
    Exit();
}
// ----------------------------------------------------------- 
// FUNCTION  MissThread::MissThread :                       
//     constructor of class MissThread                          
// PARAMETER USAGE :                                           
//    int index: assign an index to this MissThread
// FUNCTION CALLED : NONE         
// -----------------------------------------------------------
MissThread::MissThread(int index) : PassengerThread(index){
    char buff[200];  //for standard output
    ThreadName.seekp(0, ios::beg);
    ThreadName << "Miss" << index << '\0';
    sprintf(buff,"Missionaries %d starts\n",this->getIndex());
    write(1,buff,strlen(buff));
}
// ----------------------------------------------------------- 
// FUNCTION  MissThread::isCannibal :                       
//     check if this thread is a CannThread, returns 1 if true                        
// PARAMETER USAGE : NONE
// FUNCTION CALLED : NONE         
// -----------------------------------------------------------
bool MissThread::isCannibal() {
    return false;
}
// ----------------------------------------------------------- 
// FUNCTION MissThread::ThreadFunc :                       
//     ThreadFunc Class body, repeats getting onboard and offboard                   
// PARAMETER USAGE : NONE          
// FUNCTION CALLED :
//      void PassengerThread::Arrives()
//      void PassengerThread::OnBoard()      
//      void PassengerThread::OffBoard()  
//      int getDelayTime() 
// -----------------------------------------------------------
void MissThread::ThreadFunc() {
    Thread::ThreadFunc();
    while (1) {
        sleep(getDelayTime());          // take a rest
        this->Arrives();  // register to cross the river
        this->OnBoard();
        this->OffBoard();
        // come back for another river crossing
    }
    Exit();
}
// ----------------------------------------------------------- 
// FUNCTION  BoatThread::BoatThread :                       
//     constructor of class BoatThread                          
// PARAMETER USAGE :                                           
//    int b: number of required boat load
// FUNCTION CALLED : NONE         
// -----------------------------------------------------------
BoatThread::BoatThread(int b):_b(b) {
    char buff[200];  //for standard output
    ThreadName.seekp(0, ios::beg);
    ThreadName << "Boat\0";
    sprintf(buff,"***** BOAT thread starts\n");
    write(1,buff,strlen(buff));
}
// ----------------------------------------------------------- 
// FUNCTION BoatThread::ThreadFunc :                       
//     ThreadFunc Class body, repeats the trip untils boatLoad=_b                   
// PARAMETER USAGE : NONE          
// FUNCTION CALLED :
//      void PassengerThread::BoatReady()
//      void PassengerThread::BoatGo()      
//      void PassengerThread::BoatDone()
//      int getDelayTime() 
// -----------------------------------------------------------
void BoatThread::ThreadFunc() {
    Thread::ThreadFunc();
    while (1) {
        sleep(getDelayTime());         // take a rest
        BoatReady();  // ready for the next round
        BoatGo();
        sleep(getDelayTime());        // row the boat
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

// ----------------------------------------------------------- 
// FUNCTION BoatThread::BoatReady :                       
//     boat is ready to select passengers                
// PARAMETER USAGE : NONE          
// FUNCTION CALLED :
//      void RiverCrossingMonitor::boatPick()  
// -----------------------------------------------------------
void BoatThread::BoatReady(){
    char buff[200];
    sprintf(buff,"***** The boat is ready\n");
    write(1,buff,strlen(buff));
    while(!PassengerThread::RCM->boatPick()){};
}
// ----------------------------------------------------------- 
// FUNCTION BoatThread::BoatGo :                       
//     boat finishes selecting passenger, and is ready to go                
// PARAMETER USAGE : NONE          
// FUNCTION CALLED :
//      void RiverCrossingMonitor::showPickList()  
//      void RiverCrossingMonitor::getBoatLoad()  
//      void RiverCrossingMonitor::boatPickList()  
//      void RiverCrossingMonitor::baotOnBoard()  
// -----------------------------------------------------------
void BoatThread::BoatGo(){
    char buff[200];
    PassengerThread::RCM->showPickList();
    sprintf(buff,"***** Boat load (%d): Passenger list (", PassengerThread::RCM->getBoatLoad()+1);
    PassengerThread::RCM->boatPickList(buff);
    sprintf(buff+strlen(buff),")\n");
    write(1,buff,strlen(buff));
    PassengerThread::RCM->baotOnBoard();
}
// ----------------------------------------------------------- 
// FUNCTION BoatThread::BoatDone :                       
//     one boat load completes, set the passengers free               
// PARAMETER USAGE : NONE          
// FUNCTION CALLED :
//      void RiverCrossingMonitor::getBoatLoad()  
//      void RiverCrossingMonitor::boatOffBoard()  
// -----------------------------------------------------------
void BoatThread::BoatDone(){
    char buff[200];
    sprintf(buff,"***** Boat load (%d): Completed\n", PassengerThread::RCM->getBoatLoad()+1);
    write(1,buff,strlen(buff));
    PassengerThread::RCM->boatOffBoard();
}
// -----------------------------------------------------------
// FUNCTION  getDelayTime :
//     return a random delay time of 1-2secs
// PARAMETER USAGE : NONE
// FUNCTION CALLED : NONE
// -----------------------------------------------------------
int getDelayTime() {
    srand(time(NULL));
    int min = 1, max = 2;  //1sec-2sec
    int x = rand() % (max - min + 1) + min;
    return x;
}
// end of IncDec-Thrd.cpp
