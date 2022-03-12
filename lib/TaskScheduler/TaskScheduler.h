#pragma once
#include <RunnableInterface.h>
#include <SerialStream.h>


template<int MaxTasks> class TaskScheduler: public RunnableInterface
{
    private:
        RunnableAtTimeInterface* taskList[MaxTasks];
        int numTasks = 0;
        unsigned long idleCount = 0;

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
            bool didAnyTaskRun = false;

            for ( int i = 0; i < numTasks; i++ ) 
            {                
                if ( taskList[i]->canRun( nowMicros ) ) {
                    taskList[i]->run( nowMicros );                
                    didAnyTaskRun = true;
                }
            }
               
            if ( !didAnyTaskRun )
            {
                idleCount++;
            }
        }

        unsigned long getIdleCount()
        {
            return idleCount;   
        }
};