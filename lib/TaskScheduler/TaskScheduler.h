#pragma once
#include <RunnableInterface.h>
#include <SerialStream.h>
#include <IdleCounter.h>

template<int MaxTasks> class TaskScheduler: public RunnableInterface
{
    private:
        RunnableAtTimeInterface* taskList[MaxTasks];
        int numTasks = 0;
        IdleCounter idleCounter;

    public:
        void addTask( RunnableAtTimeInterface* task )
        {            
            taskList[numTasks++] = task;
        }

        int getNumberOfTasks( )
        {
            return numTasks;
        }

        RunnableAtTimeInterface* getTask( int n )
        {            
            return taskList[n];
        }

        void run( unsigned long nowMicros )
        {                               
            for ( int i = 0; i < numTasks; i++ ) 
            {                
                if ( taskList[i]->canRun( nowMicros ) ) {
                    taskList[i]->run( nowMicros );                
                    idleCounter.SignalOneTaskWasRun();
                }
            }
            idleCounter.SignelOneCycleRunAndResetTaskRunStatus();                           
        }

        unsigned long getIdleCount()
        {
            return idleCounter.getIdleCounter();   
        }
};