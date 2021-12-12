/*
 * RecurringTask.h
 *
 *  Created on: 12 juli 2020
 *      Author: Daniel
 */

#ifndef RECURRINGTASK_H_
#define RECURRINGTASK_H_

#include "RecurringTaskBase.h"

class RecurringTask: public RecurringTaskBase { 
protected:
    unsigned long int runTime;
    unsigned int rate;        
public:
	RecurringTask( ): RecurringTask(1000)
    {}
    
    RecurringTask( unsigned long int _rate ): runTime(0), rate(_rate)
    {}

    virtual bool canRun(unsigned long int now) 
    {
        return this->isRunning() && now > this->runTime;
    }   

    virtual void run(unsigned long int now)
    {        
        this->runTime += this->rate;
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

#endif /* RECURRINGTASK_H_ */
