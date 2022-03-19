#pragma once

#include <SerialStream.h>

struct HardwareClockInterface
{
    virtual void resetMicrosecondsSinceBoot() = 0;
    virtual uint32_t getMicrosecondsSinceBoot() = 0;        
    virtual void delayMicroseconds( uint32_t us ) = 0;    
};

#ifdef ARDUINO
#include <Arduino.h>
#include <HardwareInterrupts.h>

class HardwareClock: public HardwareClockInterface
{
private:    
    uint32_t timeOffset = 0;
    HardwareInterrupts hwInterrupts;

public:    
    virtual void resetMicrosecondsSinceBoot()
    {
        timeOffset = micros();
    }
    
    virtual uint32_t getMicrosecondsSinceBoot()
    {
        uint32_t rValue = micros() - timeOffset;        
        return rValue;
    }    

    virtual void delayMicroseconds( uint32_t us )
    {
        if ( us > 10000 )
            delay( us / 1000 );
        else
            delayMicroseconds( us );
    }
};

#endif

struct HardwareClockMock: public HardwareClockInterface
{
    uint32_t microsSinceStart = 0;   
    uint32_t microsToStepOnRead = 0; 

    virtual void resetMicrosecondsSinceBoot()
    {
        microsSinceStart = 0;
    }

    virtual uint32_t getMicrosecondsSinceBoot()
    {
        uint32_t oldMicros = microsSinceStart;
        microsSinceStart += microsToStepOnRead;        
        return oldMicros;
    }
    
    virtual void setMicrosToStepOnRead( uint32_t micros )
    {
        microsToStepOnRead = micros;
    }

    virtual void stepMicrosecondsSinceBoot( uint32_t us )
    {
        microsSinceStart += us;
    }

    virtual void delayMicroseconds( uint32_t us )
    {
        //Do nothing.
    }
};