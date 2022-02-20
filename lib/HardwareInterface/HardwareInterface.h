#pragma once

#ifdef ARDUINO
#include <Arduino.h>
#include <EEPROM.h>
#else
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <iostream>
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
  
  static unsigned long microsSinceBoot;
  static unsigned long microsToStepOnEachRead;

  static const int pinMaxCount = 128;
  static PinMode pinModes[pinMaxCount];
  static int pinStatuses[pinMaxCount];
  typedef void(*VoidFcnPtr)();
  static VoidFcnPtr isrList[pinMaxCount];
  
  static const int EEPROMSize = 256;
  static uint8_t EEPROMData[EEPROMSize];
  #endif 
  
  #ifndef ARDUINO
  static void resetValues()
  {
    memset(pinModes, 0, pinMaxCount*sizeof(PinMode) );
    memset(pinStatuses, 0, pinMaxCount*sizeof(PinStatus) );
    memset(EEPROMData, 0, EEPROMSize*sizeof(uint8_t) );
    memset(isrList, 0, pinMaxCount*sizeof(VoidFcnPtr) );
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
    #else
      pinStatuses[pin] = status;
      isrList[pin] = isr;
    #endif
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
    microsSinceBoot += microsToStepOnEachRead;
    return microsSinceBoot;
    #endif
  }

  static unsigned long getMillisecondsSinceBoot()
  {
    #ifdef ARDUINO
    return millis();
    #else    
    microsSinceBoot += microsToStepOnEachRead;
    return microsSinceBoot/1000;
    #endif
  }

  #ifndef ARDUINO
    static void setMicrosToStepOnEachRead( unsigned microsToStep )
    {
      microsToStepOnEachRead = microsToStep;
    }
  #endif

  static void stepMicrosecondsSinceBoot( long us )
  {
    #ifdef ARDUINO
    //Do nothing
    #else    
    microsSinceBoot += us;
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
    } else {
      return -1;
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
    if ( pin < pinMaxCount ) {
      pinStatuses[pin] = value;
    }    
    #endif
  }

  static void UpdateEEPROM( unsigned address, unsigned value )
  {
    #ifdef ARDUINO
      EEPROM.update(address, value );
    #else    
      if ( address < EEPROMSize ) {        
        int valueCapped = value & 0xFF;
        EEPROMData[address] = valueCapped;        
      }
    #endif
  }

  static unsigned int ReadEEPROM( unsigned address )
  {
    #ifdef ARDUINO
      return EEPROM.read( address );
    #else    
      if ( address < EEPROMSize ) {
        return EEPROMData[address];
      } else {
        return 0;
      }
    #endif
  }

};
