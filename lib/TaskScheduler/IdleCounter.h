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
    unsigned long maxIdleCountsPerSecond = 0;

public:
    unsigned long getIdleCounter()
    {
        return idleCounter;
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

    void Run1000IdleTaskToCalibrate()
    {            
        unsigned long startTime = hwClock->getMicrosecondsSinceBoot();
        for ( int i = 0; i < 1000; i++ ) {            
            unsigned long thisTime = hwClock->getMicrosecondsSinceBoot();
            taskScheduler->run(thisTime);            
        }
        unsigned long endTime = hwClock->getMicrosecondsSinceBoot();        
        maxIdleCountsPerSecond = idleCounter * 1e6 / (endTime - startTime);        
    }

    unsigned long getMaxIdleCountsPerSecond()
    {
        return maxIdleCountsPerSecond;
    }
};
