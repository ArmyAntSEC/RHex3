#pragma once

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <string.h> //For memset();
#endif

struct HardwarePinsInterface
{
#ifdef ARDUINO
    enum PinStatus { LOW = ::LOW, HIGH = ::HIGH, CHANGE = ::CHANGE, 
        FALLING = ::FALLING, RISING = ::RISING };
    enum PinMode { INPUT = ::INPUT, OUTPUT = ::OUTPUT, INPUT_PULLUP = ::INPUT_PULLUP, 
        INPUT_PULLDOWN  = ::INPUT_PULLDOWN };
#else
    enum PinStatus { LOW, HIGH, CHANGE, FALLING, RISING };
    enum PinMode { INPUT, OUTPUT, INPUT_PULLUP, INPUT_PULLDOWN } ;
#endif
    
    virtual void configurePin ( unsigned int pin, PinMode mode ) = 0;

    virtual void attachAnInterrupt(unsigned int pin, void(*isr)(), PinStatus status ) = 0;    

    virtual int getDigitalValueFromPin(int pin ) = 0;
  
    virtual void setDigitalValueForPin( int pin, PinStatus value ) = 0;
    
    virtual void setAnalogValueForPin( int pin, int value ) = 0;
};

#ifdef ARDUINO

struct HardwarePins: public HardwarePinsInterface
{    
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
#endif

class HardwarePinsMock: public HardwarePinsInterface
{
public:
    
    
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
        return pinStatuses[pin];
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
