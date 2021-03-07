#ifndef _RECURRINGTASK10MS_H_
#define _RECURRINGTASK10MS_H_

#include <RecurringTask.h>

class RecurringTask10ms: public RecurringTask
{
    public:
        virtual void init( unsigned long int _now )
        {
            RecurringTask::init( 10, _now );
        }
};

#endif