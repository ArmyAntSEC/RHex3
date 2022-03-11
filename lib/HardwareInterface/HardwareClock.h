#pragma once

#include <SerialStream.h>

struct HardwareClockInterface
{
    virtual void resetMicrosecondsSinceBoot() = 0;
    virtual unsigned long getMicrosecondsSinceBoot() = 0;        
    virtual void delayMicroseconds( unsigned long us ) = 0;    
};

#ifdef ARDUINO
#include <Arduino.h>
#include <HardwareInterrupts.h>

class HardwareClock: public HardwareClockInterface
{
private:    
    unsigned long timeOffset = 0;
    HardwareInterrupts hwInterrupts;

public:    
    virtual void resetMicrosecondsSinceBoot()
    {
        timeOffset = micros();
    }
    
    virtual unsigned long getMicrosecondsSinceBoot()
    {
        unsigned long rValue = micros() - timeOffset;        
        return rValue;
    }    

    virtual void delayMicroseconds( unsigned long us )
    {
        if ( us > 10000 )
            delay( us / 1000 );
        else
            delayMicroseconds( us );
    }
};

#endif

class HardwareClockMock: public HardwareClockInterface
{
private:
    unsigned long microsSinceStart = 0;   
    unsigned long microsToStepOnRead = 0; 

public:    
    virtual void resetMicrosecondsSinceBoot()
    {
        microsSinceStart = 0;
    }

    virtual unsigned long getMicrosecondsSinceBoot()
    {
        unsigned long oldMicros = microsSinceStart;
        microsSinceStart += microsToStepOnRead;        
        return oldMicros;
    }
    
    virtual void setMicrosToStepOnRead( unsigned long micros )
    {
        microsToStepOnRead = micros;
    }

    virtual void stepMicrosecondsSinceBoot( unsigned long us )
    {
        microsSinceStart += us;
    }

    virtual void delayMicroseconds( unsigned long us )
    {
        //Do nothing.
    }
};