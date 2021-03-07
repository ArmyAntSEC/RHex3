#ifndef _LEVELLOGGER_H_
#define _LEVELLOGGER_H_

#include "SerialStream.h"

#ifdef LOG_DEBUG
    #define DEBUG(x) Log << x << endl;
#else
    #define DEBUG(x) 
#endif

#define ERROR(x) Log << x << endl;


#endif