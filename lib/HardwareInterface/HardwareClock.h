#pragma once


struct HardwareClockInterface
{
    virtual void resetMicrosecondsSinceBoot() = 0;
    virtual unsigned long getMicrosecondsSinceBoot() = 0;    
};

#ifdef ARDUINO
#include <Arduino.h>

class HardwareClock: public HardwareClockInterface
{
    unsigned long timeOffset = 0;
    virtual void resetMicrosecondsSinceBoot()
    {
        timeOffset = micros();
    }
    virtual unsigned long getMicrosecondsSinceBoot()
    {
        return micros() - timeOffset;
    }    
    virtual void delayMicroseconds( unsigned long us )
    {
        if ( us > 10000 )
            delay( us / 1000 );
        else
            delayMicroseconds( us );
    }
};

#else

class HardwareClock: public HardwareClockInterface
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
    
    void setMicrosToStepOnRead( unsigned long micros )
    {
        microsToStepOnRead = micros;
    }

    void stepMicrosecondsSinceBoot( unsigned long us )
    {
        microsSinceStart += us;
    }
};

#endif