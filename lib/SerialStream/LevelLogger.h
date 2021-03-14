#ifndef _LEVELLOGGER_H_
#define _LEVELLOGGER_H_

#include "SerialStream.h"

extern const char* rootName;
const char* getName();

#ifdef DEBUG_LOG
    #define DEBUG(x) Log << "[DEBUG][" << millis() << "] " << getName() << "\t" <<  x << endl;
#else
    #define DEBUG(x)
#endif

#define ERROR(x) Log << "[ERR][" << millis() << "] " << getName() << "\t" <<  x << endl;

#define LOGGABLE(x) static const char* getName() { static const char name[] = x; return name; }    

#endif