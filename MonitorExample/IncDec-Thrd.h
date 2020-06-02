//------------------------------------------------------------------------
// Filename:
//     IncDec-Thrd.h
// PROGRAM DESCRIPTION
//     Definition file for Increment/Decrement thread classes
//------------------------------------------------------------------------

#ifndef _IncDec_Thrd_H
#define _IncDec_Thrd_H

#include "ThreadClass.h"

//------------------------------------------------------------------------
// Inc/Dec thread class definition
//------------------------------------------------------------------------

class IncDecThread: public Thread
{
     public:
          IncDecThread(int No, char ID);     // constructor
     private:
          void  ThreadFunc();
          int   number;
          char  Id;                     // Inc or Dec  key
};

#endif