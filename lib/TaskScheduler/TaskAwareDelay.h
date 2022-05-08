#pragma once

#include <HardwareClock.h>
#include <RunnableInterface.h>

class TaskAwareDelay : public TaskAwareDelayInterface
{
private:
    HardwareClockInterface *clock;
    RunnableInterface *runnable;

public:
    TaskAwareDelay(HardwareClockInterface *_clock, RunnableInterface *_runnable) : clock(_clock), runnable(_runnable)
    {
    }

    virtual void delayMicros(int32_t micros)
    {
        int32_t now = clock->getMicrosecondsSinceBoot();
        int32_t endTime = now + micros;
        while (endTime > now)
        {
            now = clock->getMicrosecondsSinceBoot();
            runnable->run(now);
        }
    }
};