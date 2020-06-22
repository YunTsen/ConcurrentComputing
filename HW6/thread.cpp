// -----------------------------------------------------------
// NAME : YunTsen Lo                         User ID: 108598056
// DUE DATE : 06/22/2020
// PROGRAM ASSIGNMENT 6
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
//   Implementation of prime number thread and master thread
// -----------------------------------------------------------

#include "thread.h"

#include <string.h>

#include <iostream>

#include "ThreadClass.h"

//global data item
int Primes[101] = {0};
Mutex mutex;//protecting the global array Primes and stdout

// ----------------------------------------------------------- 
// FUNCTION  PrimeNumberThread::PrimeNumberThread :                       
//     constructor of class PrimeNumberThread                          
// PARAMETER USAGE :                                           
//    int index: index of this primeNumberThread
//    int threadID: user-defined ID for channel declaration
// FUNCTION CALLED : NONE         
// ----------------------------------------------------------- 
PrimeNumberThread::PrimeNumberThread(int index, int threadID):Index(index) {
    char buff[200];  //for standard output
    indentation(buff);
    sprintf(buff+strlen(buff), "P%d starts and memorizes %d\n", index, index);
    write(1, buff, strlen(buff));

    ThreadName.seekp(0, ios::beg);
    ThreadName << "Sort" << index << ends;

    UserDefinedThreadID = threadID;
    neighbor = NULL;

    strstream ChannelName;
    ChannelName << "Channel" << index - 1 << "-" << index << ends;
    channel = new SynOneToOneChannel(ChannelName.str(), threadID - 1, threadID);
}

// ----------------------------------------------------------- 
// FUNCTION  PrimeNumberThread::PrimeNumberThread :                       
//     destructor of class PrimeNumberThread                          
// PARAMETER USAGE : NONE
// FUNCTION CALLED : NONE         
// ----------------------------------------------------------- 
PrimeNumberThread::~PrimeNumberThread() {
    delete channel;
}

// ----------------------------------------------------------- 
// FUNCTION PrimeNumberThread::ThreadFunc :                       
//     ThreadFunc Class body, repeats recieving a number from 
//     the predecessor and send the number to the successor
//      through channel, implements the seize program                
// PARAMETER USAGE : NONE          
// FUNCTION CALLED :
//      void indentation(char* buff);
// -----------------------------------------------------------

void PrimeNumberThread::ThreadFunc() {
    Thread::ThreadFunc();
    int number;
    char buff[200];  //for standard output
    Thread_t self = GetID();

    while (true) {
        channel->Receive(&number, sizeof(int));  // get next number
        if (number == END_OF_DATA) {             // end of data
            mutex.Lock();
            indentation(buff);
            sprintf(buff + strlen(buff), "P%d receives END \n", Index);
            write(1, buff, strlen(buff));
            mutex.Unlock();
            break;
        } else {
            mutex.Lock();
            indentation(buff);
            sprintf(buff + strlen(buff), "P%d receives %d\n", Index, number);
            write(1, buff, strlen(buff));
            mutex.Unlock();
        }
        if (number % Index != 0) {//the number is not a multiple of this->Index
            if (neighbor == NULL) {//this thread is the last thread in the chain
                mutex.Lock();
                indentation(buff);
                sprintf(buff + strlen(buff), "P%d creates %d\n", Index, number);
                write(1, buff, strlen(buff));
                mutex.Unlock();
                neighbor = new PrimeNumberThread(number, UserDefinedThreadID + 1);//creates a successor to hold this prime number
                neighbor->Begin();
            }
            neighbor->channel->Send(&number, sizeof(int));//sends number to its successor and waits for new integers from its predecessor
        } else {//the number is a multiple of this->Index(is not a prime number)
            mutex.Lock();
            indentation(buff);
            sprintf(buff + strlen(buff), "P%d ignores %d\n", Index, number);
            write(1, buff, strlen(buff));
            mutex.Unlock();
        }
    }

    Primes[Index] = Index;//save the prime number to the global array Primes[] 
    // just received END-OF-DATA, pass it to my neighbor
    if (neighbor != NULL) {
        neighbor->channel->Send(&number, sizeof(int));
        neighbor->Join();
    }
    Exit();
}

// -----------------------------------------------------------
// FUNCTION  PrimeNumberThread::indentation :
//     handles the needed indentaion for PrimeNumberThread
// PARAMETER USAGE :
//      char* buff: the buffer for standard output
// FUNCTION CALLED : NONE
// -----------------------------------------------------------
void PrimeNumberThread::indentation(char* buff) {
    memset(buff, 0, strlen(buff));
    for (int i = 0; i < this->Index; i++) {
        sprintf(buff + strlen(buff), " ");
    }
}

// -----------------------------------------------------------
// FUNCTION  MasterThread::MasterThread :
//     handles the needed indentaion for PrimeNumberThread
// PARAMETER USAGE :
//      int threadID: user-defined ID for channel declaration
//      int n: the largest integer to send to thread P2
// FUNCTION CALLED : NONE
// -----------------------------------------------------------
MasterThread::MasterThread(int threadID, int n) : _n(n) {
    char buff[200];  //for standard output
    sprintf(buff, "Master starts\n");
    write(1, buff, strlen(buff));

    UserDefinedThreadID = threadID;
    ThreadName.seekp(0, ios::beg);
    ThreadName << "Master" << ends;
}

// -----------------------------------------------------------
// FUNCTION  MasterThread::MasterThread :
//     ThreadFunc Class body, pumps a sequence of integers
//      into the synchronous channel to P2.   
// PARAMETER USAGE : NONE
// FUNCTION CALLED : NONE
// -----------------------------------------------------------
void MasterThread::ThreadFunc() {
    Thread::ThreadFunc();
    int input;
    PrimeNumberThread* firstPrimeNumberThread;  // first sorting thread

    char buff[200];  //for standard output

    firstPrimeNumberThread = new PrimeNumberThread(2, 2);  // first sorting thread
    firstPrimeNumberThread->Begin();

    for (int i = 3; i < _n + 1; i++) {
        sprintf(buff, "Master sends %d to P2\n", i);
        write(1, buff, strlen(buff));
        firstPrimeNumberThread->channel->Send(&i, sizeof(int));
    }

    // finally send END-OF-DATA
    sprintf(buff, "Master sends END\n");
    write(1, buff, strlen(buff));
    input = END_OF_DATA;
    firstPrimeNumberThread->channel->Send(&input, sizeof(int));
    firstPrimeNumberThread->Join();

    sprintf(buff, "Master prints the complete result:\n");
    write(1, buff, strlen(buff));
    for (int i = 0; i < 101; i++) {
        if (Primes[i] == 0) continue;
        sprintf(buff, "%3d", Primes[i]);
        write(1, buff, strlen(buff));
    }
    sprintf(buff, "\n");
    write(1, buff, strlen(buff));

    sprintf(buff, "Master terminates\n");
    write(1, buff, strlen(buff));
    Exit();
}

// end of thread.cpp