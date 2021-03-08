/*
 * Task.h
 *
 *  Created on: 8 juli 2020
 *      Author: Daniel
 */

#ifndef TASK_H_
#define TASK_H_

#include <LevelLogger.h>

class Task { // @suppress("Class has a virtual method and non-virtual destructor")
private:
    static const char* getNameImpl() { static const char name[] = "Task"; return name; }    
    virtual const char* getName() { return Task::getNameImpl(); }

public:
    virtual bool canRun(unsigned long int now) = 0;
    virtual void run(unsigned long int now) = 0;        
};

#endif /* TASK_H_ */
