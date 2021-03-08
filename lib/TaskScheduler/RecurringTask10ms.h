#ifndef _RECURRINGTASK10MS_H_
#define _RECURRINGTASK10MS_H_

#include <RecurringTask.h>

class RecurringTask10ms: public RecurringTask
{
    private:
        static const char* getNameImpl() { static const char name[] = "RTask10ms"; return name; }    
    public:
        virtual void init( unsigned long int _now )
        {                      
            RecurringTask::init( 10, _now );
        }
                
        virtual const char* getName()
        {   
            return RecurringTask10ms::getNameImpl();
        }
};

#endif