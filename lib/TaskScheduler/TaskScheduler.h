/*
 * TaskScheduler.h
 *
 *  Created on: 8 juli 2020
 *      Author: Daniel
 */

#ifndef TASKSCHEDULER_H_
#define TASKSCHEDULER_H_

#include "Task.h"

class TaskScheduler {
public:
    TaskScheduler();
    void add ( Task* task );
    void run();    
    void delayWithScheduler( unsigned long timeToWait );
private:
    const static int MaxTasks = 16;
    Task *tasks[MaxTasks];
    int numTasks;
};

#endif /* TASKSCHEDULER_H_ */
