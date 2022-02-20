#pragma once

#include <RunnableInterface.h>

template<int MaxTasks> class RecurringTaskGroup: public RunnableInterface
{
private:
    RunnableInterface* taskList[MaxTasks];
    int numTasks = 0;
    unsigned periodMS;
    unsigned nextRunTime = 0;

public:

    RecurringTaskGroup( unsigned _periodMS = 1000 ): periodMS(_periodMS), nextRunTime(_periodMS)
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
    
    virtual void run( unsigned long int now )
    {        
        if ( now > nextRunTime ) {
            nextRunTime += periodMS;
            for ( int i = 0; i < numTasks; i++ )
            {
                taskList[i]->run(now);
            }
        }
    }
};