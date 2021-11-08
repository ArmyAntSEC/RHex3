#pragma once

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdlib>
#include <cstring>
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
  static const int pinMaxCount = 128;
  static PinMode pinModes[pinMaxCount];
  static int pinStatuses[pinMaxCount];
  #endif
  
  #ifndef ARDUINO
  static void resetPins()
  {
    memset(pinModes, 0, pinMaxCount*sizeof(PinMode) );
    memset(pinStatuses, 0, pinMaxCount*sizeof(PinStatus) );
  }
  #endif

  static void configurePin ( unsigned int pin, PinMode mode )
  {
    #ifdef ARDUINO
    pinMode(pin, (uint8_t)mode );    
    #else
    if ( pin < pinMaxCount ) {
      pinModes[pin] = mode;
    }
    #endif //Ignore this if we have no hardware.
  }

  static void attachAnInterrupt(unsigned int pin, void(*isr)(), PinStatus status )
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
    microsSinceBoot = 0;
    #endif
  }

  static unsigned long getMicrosecondsSinceBoot()
  {
    #ifdef ARDUINO
    return micros();
    #else
    return microsSinceBoot+=10;
    #endif
  }

  static unsigned long getMillisecondsSinceBoot()
  {
    #ifdef ARDUINO
    return millis();
    #else    
    return microsSinceBoot+=10000;
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
    if ( pin < pinMaxCount ) {
      return pinStatuses[pin];
    }
    #endif
  }

  static void setDigitalValueForPin( int pin, PinStatus value )
  {
    #ifdef ARDUINO
    return digitalWrite( pin, (uint8_t)value );
    #else
    if ( pin < pinMaxCount ) {
      pinStatuses[pin] = value;
    }
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
