#pragma once

struct Runnable
{
    virtual void run(unsigned long int now) = 0;
};

template<int MaxTasks> class RecurringTaskGroup: public Runnable
{
private:
    Runnable* taskList[MaxTasks];
    int numTasks = 0;
    unsigned periodMS;
    unsigned nextRunTime = 0;

public:

    RecurringTaskGroup( unsigned _periodMS = 1000 ): periodMS(_periodMS), nextRunTime(_periodMS)
    {

    }

    void addTask( Runnable* task )
    {
        taskList[numTasks++] = task;
    }

    int getNumberOfTasks()
    {
        return numTasks;
    }

    Runnable* getTask( int n )
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