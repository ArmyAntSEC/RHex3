#pragma once

#include <RunnableInterface.h>
#include <SerialStream.h>

template<int16_t MaxTasks> class RecurringTaskGroup: public RunnableAtTimeInterface
{
private:
    RunnableInterface* taskList[MaxTasks];
    int16_t numTasks = 0;
    int32_t periodMicros;
    int32_t nextRunTimeMicros = 0;

public:

    RecurringTaskGroup( int32_t _periodMicros = 1000 ): periodMicros(_periodMicros), nextRunTimeMicros(_periodMicros)
    {

    }

    void addTask( RunnableInterface* task )
    {
        taskList[numTasks++] = task;
    }

    int16_t getNumberOfTasks()
    {
        return numTasks;
    }

    RunnableInterface* getTask( int16_t n )
    {
        return taskList[n];
    }
    
    virtual bool canRun( int32_t nowMicros )
    {        
        if ( nowMicros > nextRunTimeMicros ) {                        
            return true;
        } else {
            return false;
        }
    }

    virtual void run( int32_t nowMicros )
    {                                
        nextRunTimeMicros += periodMicros;
        for ( int16_t i = 0; i < numTasks; i++ )
        {
            taskList[i]->run(nowMicros);
        }        
    }
};