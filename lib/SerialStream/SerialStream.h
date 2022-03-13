#ifndef _SERIALSTREAM_H_
#define _SERIALSTREAM_H_

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <iostream>
#endif

#define PRINTVAR(var) " " << #var << ": " << var

enum _EndlCode { endl };

class SerialStream
{
  
public:
  template<class T>
  SerialStream& operator<< ( T arg )
  {    
    #ifdef ARDUINO
    Serial.print ( arg );
    #else
    std::cout << arg;
    #endif
    
    return *this;
  }
    
  SerialStream& operator<< (  _EndlCode )
  {
    #ifdef ARDUINO
    Serial.println ();
    #else
    std::cout << std::endl;
    #endif

    return *this;
  }
  
};
  
extern SerialStream Log;


#endif
