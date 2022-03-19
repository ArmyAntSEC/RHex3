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

    void delayMicros( int32_t micros )
    {
        int32_t now = clock->getMicrosecondsSinceBoot();
        int32_t endTime =  now + micros;
        while ( endTime > now ) {
            now = clock->getMicrosecondsSinceBoot();
            runnable->run( now );
        } 
    }
};