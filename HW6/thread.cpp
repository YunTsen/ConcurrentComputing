//------------------------------------------------------------------------
// Filename: 
//    sort-thread.cpp
// PROGRAM DESCRIPTION
//    Implementation of sorting thread and master thread
//------------------------------------------------------------------------

#include <iostream>
#include "ThreadClass.h"
#include "thread.h"

using namespace std;
// external data variable

extern SortThread *firstSortThread;  // first sorting thread

//------------------------------------------------------------------------
// SortThread::SortThread()
//    constructor    
//------------------------------------------------------------------------

SortThread::SortThread(int index, int threadID) 
{
     ThreadName.seekp(0, ios::beg);
     Index = index;
     ThreadName << "Sort" << index << ends;

     UserDefinedThreadID = threadID;

     neighbor = NULL;
     Number = NOT_DEFINED;

     strstream ChannelName;
     ChannelName << "Channel" << index - 1 << "-" << index << ends;
     channel = new SynOneToOneChannel(ChannelName.str(), threadID - 1, threadID);
}

//------------------------------------------------------------------------
// SortThread::SortThread()
//    destructor    
//------------------------------------------------------------------------

SortThread::~SortThread()
{
     delete channel;
}

//------------------------------------------------------------------------
// SortThread::ThreadFunc()
//------------------------------------------------------------------------

void SortThread::ThreadFunc()
{
     Thread::ThreadFunc();
     int number;
     int tmpNum;
     Thread_t self = GetID();

     while(true) {
          channel->Receive(&number, sizeof(int)); // get next number
          if (number == END_OF_DATA)              // end of data?
               break;
          /*if (Number == NOT_DEFINED)              // first number?
               Number = number;                   // yes, memorize it
          else {                                  // all subsequent numbers
               if (number >= Number) 
                    tmpNum = number;              // pass 'number' down
               else {
                    tmpNum = Number;              // pass the saved number
                    Number = number;              // and memorize the new one
               }
               if (neighbor == NULL){              // if I don't have a neighbor
                    neighbor = new SortThread(Index + 1, UserDefinedThreadID + 1);
                    neighbor->Begin();            // create one, run it
               }                                  // and pass the number
               neighbor->channel->Send(&tmpNum, sizeof(int));
          }*/
          if(number%Index!=0){
               if(neighbor== NULL){
                    neighbor = new SortThread(number, UserDefinedThreadID + 1);
                    neighbor->Begin();
               }
               neighbor->channel->Send(&number, sizeof(int));
          }
     }

     cout << ThreadName.str()<< " keeps --> " << Number << endl;
     // just received END-OF-DATA, pass it to my neighbor
     if (neighbor != NULL) {
          neighbor->channel->Send(&number, sizeof(int));
          neighbor->Join();
     }
     Exit();
}

//------------------------------------------------------------------------
// MasterThread::MasterThread()
//    constructor    
//------------------------------------------------------------------------

MasterThread::MasterThread(int threadID, int n):_n(n)
{
     UserDefinedThreadID = threadID;
     ThreadName.seekp(0, ios::beg);
     ThreadName << "Master" << ends;
}

//------------------------------------------------------------------------
// MasterThread::ThreadFunc()
//------------------------------------------------------------------------

void MasterThread::ThreadFunc()
{
     Thread::ThreadFunc();
     int input;

     for(int i=3;i<_n;i++){
          firstSortThread->channel->Send(&i,sizeof(int));
     }
     /*cout << "Please input all non-negative integers ended by -1: " << endl;
     do {        
          cin >> input;
          if (input == END_OF_DATA)
               break;
          else
               firstSortThread->channel->Send(&input, sizeof(int));
     } while (input != END_OF_DATA);*/

     // finally send END-OF-DATA
     input = END_OF_DATA;
     firstSortThread->channel->Send(&input, sizeof(int));
     Exit();
}

// end of Sieve.cpp