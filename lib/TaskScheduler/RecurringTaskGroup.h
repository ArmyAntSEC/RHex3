#ifndef _RECURRINGTASKGROUP_H_
#define _RECURRINGTASKGROUP_H_

#include "RecurringTask.h"

template <int MaxTasks> class RecurringTaskGroup: public RecurringTask
{
    private:        
        Task* taskList[MaxTasks];
        unsigned int numTasks = 0;
    public:
        RecurringTaskGroup(): RecurringTaskGroup(1000)
        {}

        RecurringTaskGroup( unsigned long int _rate ): RecurringTask(_rate)
        {} 

        void add ( Task* task )
        {
            if ( numTasks < MaxTasks ) {
                taskList[numTasks] = task;
                numTasks++;
            } else {
                ERROR(F("Too many tasks allocated: ") << numTasks << "/" << MaxTasks );
            }
        }
    
        void run( unsigned long int now )
        {
            RecurringTask::run(now);

            for ( unsigned int i = 0; i < numTasks; i++ ) {
                if ( taskList[i]->canRun(now) ) {
                    taskList[i]->run(now);
                }
            }
            //DEBUG( F("RecurringTaskGroup run with ") << numTasks << " items" )
        }

};

#endif