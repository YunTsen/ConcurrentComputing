#include <iostream>
#include <string.h>
#include <unistd.h>
#include <time.h> 

#include "thread.h"


using namespace std;

void BabyEagleThread::ready_to_eat(){
    char buff[200]; //for standard output
    indentation(buff);
    sprintf(buff+strlen(buff),"Baby eagle %d is ready to eat.\n", this->_index);
    write(1,buff,strlen(buff));
    EagleBaseThread::fullPotsMutex.Lock();

    /*START:check if there is no full feeding pots*/
    if(EagleBaseThread::fullPotsNum>0){//there exist full feeding pot, eat
        EagleBaseThread::eatingEagleMutex.Lock();
        EagleBaseThread::eatingEaglesNum++;
        EagleBaseThread::eatingEagleMutex.Unlock();
        EagleBaseThread::fullPotsNum--;
        EagleBaseThread::fullPotsMutex.Unlock();
        indentation(buff);
        sprintf(buff+strlen(buff),"Baby eagle %d is eating using feeding pot %d.\n", this->_index,EagleBaseThread::fullPotsNum+1);
        write(1,buff,strlen(buff));
        sleep(getDelayTime());//eating
        indentation(buff);
        sprintf(buff+strlen(buff),"Baby eagle %d finishes eating.\n", this->_index);
        write(1,buff,strlen(buff));
    }
    else if (fullPotsNum==0){//there is no full feeding pot, wake mom up
        while(1){
            int existEatingEagle;
            EagleBaseThread::eatingEagleMutex.Lock();
            existEatingEagle=EagleBaseThread::eatingEaglesNum;
            EagleBaseThread::eatingEagleMutex.Unlock();
            if(!existEatingEagle) break;
        }
        indentation(buff);
        sprintf(buff+strlen(buff),"Baby eagle %d sees all feeding pots are empty and wakes up the mother.\n", this->_index);
        write(1,buff,strlen(buff));
        EagleBaseThread::whoCallsMom=this->_index;
        EagleBaseThread::potsEmpty->Signal();//wake momEagle up
        EagleBaseThread::potsFilled->Wait();//wait for mom to fill up all feeding pots, then eat
        EagleBaseThread::eatingEagleMutex.Lock();
        EagleBaseThread::eatingEaglesNum++;
        EagleBaseThread::eatingEagleMutex.Unlock();
       
        EagleBaseThread::fullPotsNum--;
        EagleBaseThread::fullPotsMutex.Unlock();
        indentation(buff);
        sprintf(buff+strlen(buff),"Baby eagle %d is eating using feeding pot %d.\n", this->_index,EagleBaseThread::fullPotsNum+1);
        write(1,buff,strlen(buff));
        sleep(getDelayTime());//eating
        indentation(buff);
        sprintf(buff+strlen(buff),"Baby eagle %d finishes eating.\n", this->_index);
        write(1,buff,strlen(buff));
    }
    /*END:check if there is no full feeding pots*/
} 

void BabyEagleThread::finish_eating(){
    EagleBaseThread::eatingEagleMutex.Lock();
        EagleBaseThread::eatingEaglesNum--;
        EagleBaseThread::eatingEagleMutex.Unlock();
}

void BabyEagleThread::indentation(char* buff){
    memset(buff,0,strlen(buff));
    for(int i =0;i<this->_index;i++){
        sprintf(buff+strlen(buff)," ");
    }
}

void MomEagleThread::goto_sleep(){
    char buff[200]; //for standard output
    sprintf(buff,"Mother eagle takes a nap.\n");
    write(1,buff,strlen(buff));
    EagleBaseThread::potsEmpty->Wait();//wait until a baby eagle wakes her up
}

void MomEagleThread::food_ready(int m,int round){
    char buff[200]; //for standard output
    sprintf(buff,"Mother eagle is awoke by baby eagle %d and starts preparing food.\"\n",EagleBaseThread::whoCallsMom);
    write(1,buff,strlen(buff));
    sleep(getDelayTime());//preparing food
    EagleBaseThread::fullPotsNum=m;
    sprintf(buff,"Mother eagle says \"Feeding (%d)\"\n",round+1);
    write(1,buff,strlen(buff));
    EagleBaseThread::potsFilled->Signal();//notifies the calling baby "pots are refilled"
}

void MomEagleThread::retire(){
    char buff[200]; //for standard output
    sprintf(buff,"Mother eagle retires after serving %d feedings. Game is over!!!\n", this->_t);
    write(1,buff,strlen(buff));
}

int getDelayTime(){
    srand( time(NULL) );
    int min = 1,max=2;//1sec-3sec
    int x = rand() % (max - min + 1) + min;
    return x;
}