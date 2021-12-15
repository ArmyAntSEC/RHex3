#ifndef _RECURRINGTASKGROUP_H_
#define _RECURRINGTASKGROUP_H_

#include "RecurringTaskBase.h"

template <int MaxTasks> class RecurringTaskGroup: public RecurringTaskBase
{
    private:        
        Task* taskList[MaxTasks];
        unsigned int numTasks = 0;
        
        unsigned long int runTime;
        unsigned int rate;        

    public:
        RecurringTaskGroup(): RecurringTaskGroup(1000)
        {}

        RecurringTaskGroup( unsigned long int _rate ): runTime(0), rate(_rate)
        {} 

        virtual bool canRun(unsigned long int now) 
        {
            return this->isRunning() && now > this->runTime;
        }   

        void add ( Task* task )
        {
            if ( numTasks < MaxTasks ) {
                taskList[numTasks] = task;
                numTasks++;
            } else {
                //Ignore
            }
        }
    
        void run( unsigned long int now )
        {
            this->runTime += this->rate;

            for ( unsigned int i = 0; i < numTasks; i++ ) {
                if ( taskList[i]->canRun(now) ) {
                    taskList[i]->run(now);
                }
            }
        }

        virtual void init( unsigned long int _now )
        {                    	
            this->start(_now);                
        }

        virtual void start( unsigned int _now ) 
        {             
            RecurringTaskBase::start();
            this->runTime = _now + this->rate;        
        }    
    
        unsigned long int getRate() 
        { 
            return rate; 
        }
};

#endif