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
    FALLING = ::FALLING, RISING = ::RISING, UNKNOWN_STATUS };
  enum PinMode { INPUT = ::INPUT, OUTPUT = ::OUTPUT, INPUT_PULLUP = ::INPUT_PULLUP, 
    INPUT_PULLDOWN  = ::INPUT_PULLDOWN, UNKNOWN_MODE } ;
  #else
  enum PinStatus { LOW, HIGH, CHANGE, FALLING, RISING, UNKNOWN_STATUS };
  enum PinMode { INPUT, OUTPUT, INPUT_PULLUP, INPUT_PULLDOWN, UNKNOWN_MODE } ;
  
  static long int microsSinceBoot;
  static const int pinMaxCount = 128;
  static PinMode pinModes[pinMaxCount];
  static int pinStatuses[pinMaxCount];
  
  static const int EEPROMSize = 256;
  static uint8_t EEPROMData[EEPROMSize];
  #endif
  
  static void resetValues()
  {
    #ifndef ARDUINO
    memset(pinModes, 0, pinMaxCount*sizeof(PinMode) );
    memset(pinStatuses, 0, pinMaxCount*sizeof(PinStatus) );
    memset(EEPROMData, 0, EEPROMSize*sizeof(uint8_t) );
    #endif
  }
  

  static void setPinMode ( unsigned int pin, PinMode mode )
  {
    #ifdef ARDUINO
    pinMode(pin, (uint8_t)mode );    
    #else
    if ( pin < pinMaxCount ) {
      pinModes[pin] = mode;
    }
    #endif 
  }

  static PinMode getPinMode ( unsigned int pin )
  {
    #ifdef ARDUINO
    uint8_t bit = digitalPinToBitMask(pin);
    uint8_t port = digitalPinToPort(pin);
    volatile uint8_t *reg = portModeRegister(port);

    if (*reg & bit) {
      return PinMode::OUTPUT;
    } else {
      return PinMode::INPUT;
    }  

    #else
    if ( pin < pinMaxCount ) {
      return pinModes[pin];
    } else {
      return PinMode::UNKNOWN_MODE;
    }
    #endif
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
    std::cout << "Microseconds: " << microsSinceBoot << std::endl;
    return microsSinceBoot+=10;
    #endif
  }

  static unsigned long getMillisecondsSinceBoot()
  {
    #ifdef ARDUINO
    return millis();
    #else        
    microsSinceBoot += 10000;
    return microsSinceBoot/1000;
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
