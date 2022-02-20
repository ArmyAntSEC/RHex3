#pragma once
#include <Runnable.h>

struct RunnableAtTime
{
    virtual void run(unsigned long now) = 0;
    virtual bool canRun( unsigned long now) = 0;
};

template<int MaxTasks> class TaskScheduler: public Runnable
{
    private:
        RunnableAtTime* taskList[MaxTasks];
        int numTasks = 0;

    public:
        void addTask( RunnableAtTime* task )
        {            
            taskList[numTasks++] = task;
        }

        int getNumberOfTasks( )
        {
            return numTasks;
        }

        RunnableAtTime* getTask( int n )
        {            
            return taskList[n];
        }

        void run( unsigned long now )
        {            
            for ( int i = 0; i < numTasks; i++ ) 
            {                
                if ( taskList[i]->canRun( now ) )
                    taskList[i]->run( now );
            }   
        }
};