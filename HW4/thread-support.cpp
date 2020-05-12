#include <iostream>
#include <string.h>
#include <unistd.h>
#include <time.h> 

#include "thread.h"


using namespace std;

void BabyEagleThread::ready_to_eat(){
    char buff[200]; //for standard output
    sprintf(buff,"Baby eagle %d is ready to eat.", this->_index);
    write(1,buff,strlen(buff));
} 

void BabyEagleThread::finish_eating(){

}

void MomEagleThread::goto_sleep(){

}

void MomEagleThread::food_ready(int m){
    char buff[200]; //for standard output
    fullPots=m;
    sprintf(buff,"Mother eagle says \"Feeding (round)\"");
    write(1,buff,strlen(buff));
    potsFilled.signal();
}

int delayTime(){
    srand( time(NULL) );
    int min = 1,max=3;//1sec-3sec
    int x = rand() % (max - min + 1) + min;
    return x;
}