#ifndef _SERIALSTREAM_H_
#define _SERIALSTREAM_H_

#ifdef ARDUINO

#include <Arduino.h>

enum _EndlCode { endl };

class SerialStream
{
  
public:
  SerialStream& operator<< ( char const * str )
  {
    Serial.print ( str );
    return *this;
  }
  
  SerialStream& operator<< ( String str )
  {
    Serial.print ( str );
    return *this;
  }
  
  SerialStream& operator<< ( long int num )
  {
    Serial.print ( num );
    return *this;
  }
  
  SerialStream& operator<< ( long unsigned int num )
  {
    Serial.print ( num );
    return *this;
  }
  
  SerialStream& operator<< ( int num )
  {
    Serial.print ( num );
    return *this;
  }
  
  SerialStream& operator<< ( unsigned int num )
  {
    Serial.print ( num );
    return *this;
  }
  
  SerialStream& operator<< ( double num )
  {
    Serial.print ( num );
    return *this;
  }
  
  SerialStream& operator<< (  _EndlCode )
  {
    Serial.println ();
    return *this;
  }
  
};
  
extern SerialStream Log;

#else

#include <iostream>
#define Log std::cout

#endif

#endif
