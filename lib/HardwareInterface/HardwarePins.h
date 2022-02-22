#pragma once

#ifdef ARDUINO
#include <Arduino.h>

class HardwarePins
{
public:
    enum PinStatus { LOW = ::LOW, HIGH = ::HIGH, CHANGE = ::CHANGE, 
        FALLING = ::FALLING, RISING = ::RISING };
    enum PinMode { INPUT = ::INPUT, OUTPUT = ::OUTPUT, INPUT_PULLUP = ::INPUT_PULLUP, 
        INPUT_PULLDOWN  = ::INPUT_PULLDOWN };
    
    void configurePin ( unsigned int pin, PinMode mode )
    {
        pinMode(pin, (uint8_t)mode );
    }

    void attachAnInterrupt(unsigned int pin, void(*isr)(), PinStatus status )
    {            
        attachInterrupt(digitalPinToInterrupt(pin), isr, ::PinStatus(status) );    
    }

    int getDigitalValueFromPin(int pin )
    {    
        return digitalRead( pin );
    }
  
    void setDigitalValueForPin( int pin, PinStatus value )
    {    
        digitalWrite( pin, (uint8_t)value );
    }
  
    void setAnalogValueForPin( int pin, int value )
    {    
        analogWrite( pin, value );
    }  
};
#else
#include <iostream>

class HardwarePins
{
public:
    enum PinStatus { LOW, HIGH, CHANGE, FALLING, RISING };
    enum PinMode { INPUT, OUTPUT, INPUT_PULLUP, INPUT_PULLDOWN } ;
    
    static const int pinMaxCount = 128;
    typedef void(*VoidFcnPtr)();
    VoidFcnPtr isrList[pinMaxCount];
    int pinStatuses[pinMaxCount];
    int pinModes[pinMaxCount];

    void resetValues()
    {
        memset ( isrList, 0, pinMaxCount * sizeof(VoidFcnPtr) );
        memset ( pinStatuses, 0, pinMaxCount * sizeof(int) );
    }

    void configurePin ( unsigned int pin, PinMode mode )
    {        
        pinModes[pin] = mode;
    }

    void attachAnInterrupt(unsigned int pin, void(*isr)(), PinStatus status )
    {    
        isrList[pin] = isr;
        pinStatuses[pin] = status;
    }

    int getDigitalValueFromPin(int pin )
    {    
        std::cerr << "Implement getDigitalValueFromPin!" << std::endl;
        return 0;
    }
  
    void setDigitalValueForPin( int pin, PinStatus value )
    {     
        pinStatuses[pin] = value;
    }
  
    void setAnalogValueForPin( int pin, int value )
    {            
        pinStatuses[pin] = value;
    }  
};

#endif