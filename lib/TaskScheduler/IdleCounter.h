#pragma once

class IdleCounter
{
private:
    unsigned long idleCounter = 0;
    bool taskWasRun = false;
public:
    unsigned long getIdleCounter()
    {
        return idleCounter;
    }

    void SignelOneCycleRunAndResetTaskRunStatus()
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
};
