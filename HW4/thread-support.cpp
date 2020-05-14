// -----------------------------------------------------------
// NAME : YunTsen Lo                         User ID: 108598056
// DUE DATE : 05/20/2020
// PROGRAM ASSIGNMENT 4
// FILE NAME : thread-support.cpp
// PROGRAM PURPOSE :
//    Parts of class implemention file for class EagleBaseThread &
//          BabyEagleThread & MomEagleThread (supporting functions)
// -----------------------------------------------------------

#include <string.h>
#include <time.h>
#include <unistd.h>

#include <iostream>

#include "thread.h"

using namespace std;

// -----------------------------------------------------------
// FUNCTION  BabyEagleThread::ready_to_eat :
//     babyEagle can eat while there is available feeding pot
//      (add one to counter of eating babies),
//      otherwise wake up its mom for refilled pots
// PARAMETER USAGE : NONE
// FUNCTION CALLED : NONE
// -----------------------------------------------------------
void BabyEagleThread::ready_to_eat() {
    char buff[200];  //for standard output
    indentation(buff);
    sprintf(buff + strlen(buff), "Baby eagle %d is ready to eat.\n", this->_index);
    write(1, buff, strlen(buff));
    EagleBaseThread::fullPotsMutex.Lock();

    //**START:check if there is no full feeding pots**//
    if (EagleBaseThread::fullPotsNum > 0) {  //there exist full feeding pot, eat
        EagleBaseThread::eatingEagleMutex.Lock();
        EagleBaseThread::eatingEaglesNum++;  //someone's eating
        EagleBaseThread::eatingEagleMutex.Unlock();
        EagleBaseThread::fullPotsNum--;  //a feeding pot is used
        EagleBaseThread::fullPotsMutex.Unlock();
    } else if (fullPotsNum == 0) {  //there is no full feeding pot, wake mom up
        /*START:check if there exists eating babyEagle*/
        while (1) {
            int existEatingEagle;
            EagleBaseThread::eatingEagleMutex.Lock();
            existEatingEagle = EagleBaseThread::eatingEaglesNum;
            EagleBaseThread::eatingEagleMutex.Unlock();
            if (!existEatingEagle) break;  //no eating babies, can wake mom up now
        }
        /*END:check if there exists eating babyEagle*/
        indentation(buff);
        sprintf(buff + strlen(buff), "Baby eagle %d sees all feeding pots are empty and wakes up the mother.\n", this->_index);
        write(1, buff, strlen(buff));
        EagleBaseThread::whoCallsMom = this->_index;
        EagleBaseThread::potsEmpty->Signal();  //wake momEagle up
        EagleBaseThread::potsFilled->Wait();   //wait for mom to fill up all feeding pots, then eat
        EagleBaseThread::eatingEagleMutex.Lock();
        EagleBaseThread::eatingEaglesNum++;  //someone's eating
        EagleBaseThread::eatingEagleMutex.Unlock();
        EagleBaseThread::fullPotsNum--;  //a feeding pot is used
        EagleBaseThread::fullPotsMutex.Unlock();
    }
    //**END:check if there is no full feeding pots**//
}

// -----------------------------------------------------------
// FUNCTION  BabyEagleThread::finish_eating :
//     babyEagle eats and minus the counter of eating babies afterwards
// PARAMETER USAGE : NONE
// FUNCTION CALLED : 
//      void getDelayTime()
// -----------------------------------------------------------
void BabyEagleThread::finish_eating() {
    char buff[200];  //for standard output
    indentation(buff);
    sprintf(buff + strlen(buff), "Baby eagle %d is eating using feeding pot %d.\n", this->_index, EagleBaseThread::fullPotsNum + 1);
    write(1, buff, strlen(buff));
    sleep(getDelayTime());  //eating
    indentation(buff);
    sprintf(buff + strlen(buff), "Baby eagle %d finishes eating.\n", this->_index);
    write(1, buff, strlen(buff));
    EagleBaseThread::eatingEagleMutex.Lock();
    EagleBaseThread::eatingEaglesNum--;  //someone finishes eating
    EagleBaseThread::eatingEagleMutex.Unlock();
}

// -----------------------------------------------------------
// FUNCTION  BabyEagleThread::indentation :
//     handles the needed indentaion for babyEagle
// PARAMETER USAGE :
//      char* buff: the buffer for standard output
// FUNCTION CALLED : NONE
// -----------------------------------------------------------
void BabyEagleThread::indentation(char* buff) {
    memset(buff, 0, strlen(buff));
    for (int i = 0; i < this->_index; i++) {
        sprintf(buff + strlen(buff), " ");
    }
}

// -----------------------------------------------------------
// FUNCTION  MomEagleThread::goto_sleep :
//     momEagle sleeps until her baby wake her up
// PARAMETER USAGE : NONE
// FUNCTION CALLED : 
//      void getDelayTime()
// -----------------------------------------------------------
void MomEagleThread::goto_sleep() {
    char buff[200];  //for standard output
    sprintf(buff, "Mother eagle takes a nap.\n");
    write(1, buff, strlen(buff));
    EagleBaseThread::potsEmpty->Wait();  //wait until a baby eagle wakes her up
}

// -----------------------------------------------------------
// FUNCTION  MomEagleThread::food_ready :
//     momEagle is awake, refills all feeding pots and notifies the calling child
// PARAMETER USAGE : NONE
// FUNCTION CALLED : 
//      void getDelayTime()
// ----------------------------------------------------------
void MomEagleThread::food_ready(int m, int round) {
    char buff[200];  //for standard output
    sprintf(buff, "Mother eagle is awoke by baby eagle %d and starts preparing food.\"\n", EagleBaseThread::whoCallsMom);
    write(1, buff, strlen(buff));
    sleep(getDelayTime());  //preparing food
    EagleBaseThread::fullPotsNum = m;
    sprintf(buff, "Mother eagle says \"Feeding (%d)\"\n", round + 1);
    write(1, buff, strlen(buff));
    EagleBaseThread::potsFilled->Signal();  //notifies the calling baby "pots are refilled"
}

// -----------------------------------------------------------
// FUNCTION  MomEagleThread::retire :
//     momEagle retires, exit the program
// PARAMETER USAGE : NONE
// FUNCTION CALLED : NONE
// -----------------------------------------------------------
void MomEagleThread::retire() {
    char buff[200];  //for standard output
    sprintf(buff, "Mother eagle retires after serving %d feedings. Game is over!!!\n", this->_t);
    write(1, buff, strlen(buff));
    exit(0);
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