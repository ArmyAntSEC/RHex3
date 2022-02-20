#pragma once

#include <HardwareClock.h>
#include <RunnableInterface.h>

class TaskAwareDelay
{
private:
    HardwareClockInterface* clock;
    RunnableInterface* runnable;

public:
    TaskAwareDelay( HardwareClockInterface* _clock, RunnableInterface* _runnable ): clock(_clock), runnable(_runnable)
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