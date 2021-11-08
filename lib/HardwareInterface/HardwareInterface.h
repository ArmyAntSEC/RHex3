#pragma once

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdlib>
#endif

class HardwareInterface
{
  public:
  #ifdef ARDUINO
  enum PinStatus { LOW = ::LOW, HIGH = ::HIGH, CHANGE = ::CHANGE, 
    FALLING = ::FALLING, RISING = ::RISING };
  enum PinMode { INPUT = ::INPUT, OUTPUT = ::OUTPUT, INPUT_PULLUP = ::INPUT_PULLUP, 
    INPUT_PULLDOWN  = ::INPUT_PULLDOWN } ;
  #else
  enum PinStatus { LOW, HIGH, CHANGE, FALLING, RISING };
  enum PinMode { INPUT, OUTPUT, INPUT_PULLUP, INPUT_PULLDOWN } ;
  static long int microsSinceBoot;
  #endif
  
  static void configurePin ( unsigned int pin, HardwareInterface::PinMode mode )
  {
    #ifdef ARDUINO
    pinMode(pin, (uint8_t)mode );    
    #endif //Ignore this if we have no hardware.
  }

  static void attachAnInterrupt(unsigned int pin, void(*isr)(), HardwareInterface::PinStatus status )
  {            
    #ifdef ARDUINO
    attachInterrupt(digitalPinToInterrupt(pin), isr, ::PinStatus(status) );    
    #endif //Ignore this if we have no hardware
  }

  static void disableInterrupts()
  {
    #ifdef ARDUINO
    noInterrupts();
    #endif
  }

  static void enableInterrupts()
  {
    #ifdef ARDUINO
    interrupts();
    #endif
  }

  static void resetMicrosecondsSinceBoot()
  {
    #ifdef ARDUINO
    //Do nothing
    #else
    HardwareInterface::microsSinceBoot = 0;
    #endif
  }

  static unsigned long getMicrosecondsSinceBoot()
  {
    #ifdef ARDUINO
    return micros();
    #else
    return HardwareInterface::microsSinceBoot+=10;
    #endif
  }

  static unsigned long getMillisecondsSinceBoot()
  {
    #ifdef ARDUINO
    return millis();
    #else    
    return HardwareInterface::microsSinceBoot+=10000;
    #endif
  }

  static void delayForMilliseconds( unsigned long milliseconds )
  {
    #ifdef ARDUINO
    delay(2000);
    #endif
  }

  static int getDigitalValueFromPin(int pin )
  {
    #ifdef ARDUINO
    return digitalRead( pin );
    #else
    return 0;
    #endif
  }

  static void setDigitalValueForPin( int pin, PinStatus value )
  {
    #ifdef ARDUINO
    return digitalWrite( pin, (uint8_t)value );
    #else
    //Do nothing.
    #endif
  }

  static void setAnalogValueForPin( int pin, int value )
  {
    #ifdef ARDUINO
    return analogWrite( pin, value );
    #else
    //Do nothing.
    #endif
  }

};
