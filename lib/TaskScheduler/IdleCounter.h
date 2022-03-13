#pragma once

#include <HardwareClock.h>
#include <RunnableInterface.h>

class IdleCounter
{
private:
    unsigned long idleCounter = 0;
    bool taskWasRun = false;
    RunnableInterface* taskScheduler;
    HardwareClockInterface* hwClock;    
    unsigned long lastMeasurementTimeMicros = 0;

public:
    unsigned long getIdleCounter()
    {
        return idleCounter;
    }

    unsigned long getIdleCountsPerSecondAndResetCounter()
    {
        unsigned long thisTime = hwClock->getMicrosecondsSinceBoot();
        unsigned long timeDelta = thisTime - lastMeasurementTimeMicros;
        lastMeasurementTimeMicros = thisTime;
        unsigned long thisIdleCounter = idleCounter;
        idleCounter = 0;
        return thisIdleCounter * 1e6 / timeDelta;
    }

    void SignalOneCycleRunAndResetTaskRunStatus()
    {
        if ( !taskWasRun ) {
            idleCounter++;
        }
        taskWasRun = false;
    }

    void SignalOneTaskWasRun()
    {
        taskWasRun = true;
    }    

    void setTaskSchedulerAndClock( RunnableInterface* _taskScheduler, HardwareClockInterface* _hwClock )
    {
        taskScheduler = _taskScheduler;
        hwClock = _hwClock;
    }

    unsigned long Run1000IdleTaskToCalibrateAndGetMaxIdleCountsPerSecond()
    {            
        getIdleCountsPerSecondAndResetCounter();
        for ( int i = 0; i < 1000; i++ ) {            
            unsigned long thisTime = hwClock->getMicrosecondsSinceBoot();
            taskScheduler->run(thisTime);            
        }
        return getIdleCountsPerSecondAndResetCounter();
    }
};
