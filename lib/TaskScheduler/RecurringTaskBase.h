#ifndef _RECURRINGTASKEVERY_H_
#define _RECURRINGTASKEVERY_H_

#include <Task.h>

class RecurringTaskBase: public Task
{
private:
    bool running = false;
    LOGGABLE( "RTaskBase");    

public:
    virtual bool canRun(unsigned long int ) 
    {        
        return this->running;
    }   

    virtual void init()
    {        
        this->start();                
    }

    virtual void start() 
    {         
        this->running = true;         
    }    
    
    virtual void stop() 
    { 
        this->running = false; 
    }

    bool isRunning() 
    {
        return this->running;
    }
};

#endif