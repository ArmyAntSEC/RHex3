#pragma once
#include <RunnableInterface.h>
#include <SerialStream.h>
#include <IdleCounter.h>

class TaskScheduler: public RunnableInterface
{
    private:
        static const int MaxTasks = 6;
        RunnableAtTimeInterface* taskList[MaxTasks];
        int numTasks = 0;
        IdleCounter* idleCounter = 0;
        
    public:
        
        void addTask( RunnableAtTimeInterface* task )
        {            
            taskList[numTasks++] = task;
        }

        void setIdleCounter( IdleCounter* _idleCounter )
        {
            idleCounter = _idleCounter;
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
                    idleCounter->SignalOneTaskWasRun();
                }
            }
            idleCounter->SignalOneCycleRunAndResetTaskRunStatus();                           
        }        
};