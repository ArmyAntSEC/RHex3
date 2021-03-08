/*
 * RecurringTask.h
 *
 *  Created on: 12 juli 2020
 *      Author: Daniel
 */

#ifndef RECURRINGTASK_H_
#define RECURRINGTASK_H_

#include "Task.h"
#include <LevelLogger.h>

class RecurringTask: public Task { // @suppress("Class has a virtual method and non-virtual destructor")
protected:
    unsigned long int runTime;
    unsigned int rate;
    bool running;
    static const char* getNameImpl() { static const char name[] = "RecTask"; return name; }    
public:
	RecurringTask( ): RecurringTask(1000)
    {}
    
    RecurringTask( unsigned long int _rate ): runTime(0), rate(_rate), running(true)
    {}

    virtual bool canRun(unsigned long int now) 
    {
        //ERROR(F("Test: Running: ") << this->running << " Now: " << now << " Next run:" << this->runTime << " Rate:" << this->rate );
        return this->running && now > this->runTime;
    }   

    virtual void run(unsigned long int now)
    {        
        this->runTime += this->rate;
    }

    virtual void init( unsigned int _rate, unsigned long int _now )
    {        
        this->rate = _rate;	        
    	DEBUG(F("Init with rate: ") << this->rate << F(" at time ") << _now );
        this->start(_now);                
    }

    void start( unsigned int _now ) 
    {         
        this->running = true; 
        this->runTime = _now + this->rate;
        DEBUG(F("Started with next runtime: ") << this->runTime );
    }    
    
    void stop() 
    { 
        this->running = false; 
    }

    unsigned long int getRate() 
    { 
        return rate; 
    }

    bool isRunning() 
    {
        return running;
    }
    
    virtual const char* getName()
    {
        return RecurringTask::getNameImpl();
    }
};

#endif /* RECURRINGTASK_H_ */
