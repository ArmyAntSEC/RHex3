#pragma once
#include <base.h>
#include <SerialStream.h>

struct HardwareClockInterface
{
    virtual void resetMicrosecondsSinceBoot() = 0;
    virtual int32_t getMicrosecondsSinceBoot() = 0;        
    virtual void delayMicroseconds( int32_t us ) = 0;    
};

#ifdef ARDUINO
#include <HardwareInterrupts.h>

class HardwareClock: public HardwareClockInterface
{
private:    
    int32_t timeOffset = 0;
    HardwareInterrupts hwInterrupts;

public:    
    virtual void resetMicrosecondsSinceBoot()
    {
        timeOffset = micros();
    }
    
    virtual int32_t getMicrosecondsSinceBoot()
    {
        int32_t rValue = micros() - timeOffset;        
        return rValue;
    }    

    virtual void delayMicroseconds( int32_t us )
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
    int32_t microsSinceStart = 0;   
    int32_t microsToStepOnRead = 0; 

    virtual void resetMicrosecondsSinceBoot()
    {
        microsSinceStart = 0;
    }

    virtual int32_t getMicrosecondsSinceBoot()
    {
        int32_t oldMicros = microsSinceStart;
        microsSinceStart += microsToStepOnRead;        
        return oldMicros;
    }
    
    virtual void setMicrosToStepOnRead( int32_t micros )
    {
        microsToStepOnRead = micros;
    }

    virtual void stepMicrosecondsSinceBoot( int32_t us )
    {
        microsSinceStart += us;
    }

    virtual void delayMicroseconds( int32_t us )
    {
        //Do nothing.
    }
};