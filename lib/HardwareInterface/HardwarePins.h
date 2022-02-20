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
      
    void configurePin ( unsigned int pin, PinMode mode )
    {        
        std::cerr << "Implement me!" << std::endl;
    }

    void attachAnInterrupt(unsigned int pin, void(*isr)(), PinStatus status )
    {    
        std::cerr << "Implement me!" << std::endl;                
    }

    int getDigitalValueFromPin(int pin )
    {    
        std::cerr << "Implement me!" << std::endl;
        return 0;
    }
  
    void setDigitalValueForPin( int pin, PinStatus value )
    {     
        std::cerr << "Implement me!" << std::endl;       
    }
  
    void setAnalogValueForPin( int pin, int value )
    {            
        std::cerr << "Implement me!" << std::endl;
    }  
};

#endif