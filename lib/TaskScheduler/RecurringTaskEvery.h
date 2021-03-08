#ifndef _RECURRINGTASKEVERY_H_
#define _RECURRINGTASKEVERY_H_

#include <Task.h>

class RecurringTaskEvery: public Task
{
private:
    bool running = false;
    static const char* getNameImpl() { static const char name[] = "RTaskEvry"; return name; }    
    virtual const char* getName() { return RecurringTaskEvery::getNameImpl(); }

public:
    virtual bool canRun(unsigned long int ) 
    {        
        return this->running;
    }   

    virtual void init()
    {        
        this->start();                
    }

    void start() 
    {         
        this->running = true;         
    }    
    
    void stop() 
    { 
        this->running = false; 
    }

    bool isRunning() 
    {
        return this->running;
    }
};

#endif