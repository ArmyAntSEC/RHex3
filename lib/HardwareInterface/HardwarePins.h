#pragma once
#include <base.h>

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
    
    virtual void configurePin (  uint16_t pin, PinMode mode ) = 0;

    virtual void attachAnInterrupt( uint16_t pin, void(*isr)(), PinStatus status ) = 0;    

    virtual int16_t getDigitalValueFromPin(int16_t pin ) = 0;
  
    virtual void setDigitalValueForPin( int16_t pin, PinStatus value ) = 0;
    
    virtual void setAnalogValueForPin( int16_t pin, int16_t value ) = 0;
};

#ifdef ARDUINO

struct HardwarePins: public HardwarePinsInterface
{    
    void configurePin (  uint16_t pin, PinMode mode )
    {
        pinMode(pin, (uint8_t)mode );
    }

    void attachAnInterrupt( uint16_t pin, void(*isr)(), PinStatus status )
    {            
        attachInterrupt(digitalPinToInterrupt(pin), isr, ::PinStatus(status) );    
    }

    int16_t getDigitalValueFromPin(int16_t pin )
    {    
        return digitalRead( pin );
    }
  
    void setDigitalValueForPin( int16_t pin, PinStatus value )
    {    
        digitalWrite( pin, (uint8_t)value );
    }
  
    void setAnalogValueForPin( int16_t pin, int16_t value )
    {    
        analogWrite( pin, value );
    }  
};
#endif

class HardwarePinsMock: public HardwarePinsInterface
{
public:
    
    
    static const int16_t pinMaxCount = 128;
    typedef void(*VoidFcnPtr)();
    VoidFcnPtr isrList[pinMaxCount];
    int16_t pinStatuses[pinMaxCount];
    int16_t pinModes[pinMaxCount];

    void resetValues()
    {
        memset ( isrList, 0, pinMaxCount * sizeof(VoidFcnPtr) );
        memset ( pinStatuses, 0, pinMaxCount * sizeof(int) );
    }

    void configurePin ( uint16_t pin, PinMode mode )
    {        
        pinModes[pin] = mode;
    }

    void attachAnInterrupt(uint16_t pin, void(*isr)(), PinStatus status )
    {    
        isrList[pin] = isr;
        pinStatuses[pin] = status;
    }

    int16_t getDigitalValueFromPin(int16_t pin )
    {            
        return pinStatuses[pin];
    }
  
    void setDigitalValueForPin( int16_t pin, PinStatus value )
    {     
        pinStatuses[pin] = value;
    }
  
    void setAnalogValueForPin( int16_t pin, int16_t value )
    {            
        pinStatuses[pin] = value;
    }  
};
