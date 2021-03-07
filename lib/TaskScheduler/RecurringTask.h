/*
 * RecurringTask.h
 *
 *  Created on: 12 juli 2020
 *      Author: Daniel
 */

#ifndef RECURRINGTASK_H_
#define RECURRINGTASK_H_

#include "Task.h"

class RecurringTask: public Task { // @suppress("Class has a virtual method and non-virtual destructor")
private:
    unsigned long int runTime;
    unsigned int rate;
    bool running;

public:
	RecurringTask( ): runTime(0), rate(1000), running(true)
    {}

    bool canRun(unsigned long int now) 
    {
        return this->running && now > this->runTime;
    }   

    virtual void run(unsigned long int now)
    {
        this->runTime += this->rate;
    }

    virtual void init( unsigned int _rate, unsigned long int _now )
    {
        this->rate = _rate;	
    	this->start(_now);
    }

    void start( unsigned int _now ) 
    { 
        this->running = true; 
        this->runTime = _now + this->rate;
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
};

#endif /* RECURRINGTASK_H_ */
