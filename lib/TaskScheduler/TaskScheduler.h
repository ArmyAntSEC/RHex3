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
private:
    const static int MaxTasks = 16;
    Task *tasks[MaxTasks];
    int numTasks;
    unsigned long int averageLoopTime;
    static const unsigned int filterFactor = 100;       
    unsigned long int nextRunTime = 0;  
};

#endif /* TASKSCHEDULER_H_ */
