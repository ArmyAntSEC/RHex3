#pragma once

#include <HardwareClock.h>
#include <Runnable.h>

class TaskAwareDelay
{
private:
    HardwareClockInterface* clock;
    Runnable* runnable;

public:
    TaskAwareDelay( HardwareClockInterface* _clock, Runnable* _runnable ): clock(_clock), runnable(_runnable)
    {}

    void delayMicros( unsigned long micros )
    {
        unsigned long now = clock->getMicrosecondsSinceBoot();
        unsigned long endTime =  now + micros;
        while ( endTime > now ) {
            now = clock->getMicrosecondsSinceBoot();
            runnable->run( now );
        } 
    }
};