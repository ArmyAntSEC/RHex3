#pragma once
#include <cstdint>
#include <RunnableInterface.h>
#include <SerialStream.h>
#include <IdleCounter.h>
#include <HardwareClock.h>

class TaskScheduler: public RunnableInterface
{
    private:
        static const int16_t MaxTasks = 6;
        RunnableAtTimeInterface* taskList[MaxTasks];
        int16_t numTasks = 0;
        IdleCounter idleCounter;
        HardwareClockInterface* hwClock;
        
    public:
        TaskScheduler( HardwareClockInterface* _hwClock ): hwClock(_hwClock), idleCounter( this, _hwClock )
        { }

        void addTask( RunnableAtTimeInterface* task )
        {            
            taskList[numTasks++] = task;
        }

        IdleCounter* getIdleCounterObject( )
        {
            return &idleCounter;
        }

        int16_t getNumberOfTasks( )
        {
            return numTasks;
        }

        RunnableAtTimeInterface* getTask( int16_t n )
        {            
            return taskList[n];
        }

        void run( uint32_t nowMicros )
        {                               
            for ( int16_t i = 0; i < numTasks; i++ ) 
            {                
                if ( taskList[i]->canRun( nowMicros ) ) {
                    taskList[i]->run( nowMicros );                
                    idleCounter.SignalOneTaskWasRun();
                }
            }
            idleCounter.SignalOneCycleRunAndResetTaskRunStatus();                           
        }        
};