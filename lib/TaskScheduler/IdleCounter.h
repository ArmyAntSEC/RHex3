#pragma once

#include <HardwareClock.h>
#include <RunnableInterface.h>

class IdleCounter
{
private:
    uint32_t idleCounter = 0;
    bool taskWasRun = false;
    RunnableInterface* taskScheduler;
    HardwareClockInterface* hwClock;    
    uint32_t lastMeasurementTimeMicros = 0;
    uint32_t maxIdleCountsPerSecond = 1e6; //large number

public:

    IdleCounter( RunnableInterface* _scheduler, HardwareClockInterface* _hwClock ): 
        taskScheduler(_scheduler), hwClock(_hwClock)
    {}
    
    uint32_t getIdleCounter()
    {
        return idleCounter;
    }

    int16_t getCPUFactorPercent()
    {
        return idleCounter * 100 / maxIdleCountsPerSecond;
    }
    
    uint32_t getIdleCountsPerSecondAndResetCounter()
    {
        uint32_t thisTime = hwClock->getMicrosecondsSinceBoot();
        uint32_t timeDelta = thisTime - lastMeasurementTimeMicros;
        lastMeasurementTimeMicros = thisTime;
        uint32_t thisIdleCounter = idleCounter;
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

    void Run1000IdleTaskToCalibrateAndGetMaxIdleCountsPerSecond()
    {            
        getIdleCountsPerSecondAndResetCounter();
        for ( int16_t i = 0; i < 1000; i++ ) {            
            uint32_t thisTime = hwClock->getMicrosecondsSinceBoot();
            taskScheduler->run(thisTime);            
        }
        maxIdleCountsPerSecond = getIdleCountsPerSecondAndResetCounter();
    }
};
