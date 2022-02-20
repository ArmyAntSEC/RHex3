#pragma once

#include <RunnableInterface.h>

template<int MaxTasks> class RecurringTaskGroup: public RunnableAtTimeInterface
{
private:
    RunnableInterface* taskList[MaxTasks];
    int numTasks = 0;
    unsigned periodMicros;
    unsigned nextRunTimeMicros = 0;

public:

    RecurringTaskGroup( unsigned _periodMicros = 1000 ): periodMicros(_periodMicros), nextRunTimeMicros(_periodMicros)
    {

    }

    void addTask( RunnableInterface* task )
    {
        taskList[numTasks++] = task;
    }

    int getNumberOfTasks()
    {
        return numTasks;
    }

    RunnableInterface* getTask( int n )
    {
        return taskList[n];
    }
    
    virtual bool canRun( unsigned long nowMicros )
    {
        if ( nowMicros > nextRunTimeMicros ) {
            nextRunTimeMicros += periodMicros;        
            return true;
        } else {
            return false;
        }
    }

    virtual void run( unsigned long nowMicros )
    {                
        nextRunTimeMicros += periodMicros;
        for ( int i = 0; i < numTasks; i++ )
        {
            taskList[i]->run(nowMicros);
        }        
    }
};