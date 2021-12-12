/*
 * TaskScheduler.cpp
 *
 *  Created on: 8 juli 2020
 *      Author: Daniel
 */

#include "TaskScheduler.h"
#include <SerialStream.h>
#include <HardwareInterface.h>

#ifdef ARDUINO
#include <stdio.h> //For memset
#else
#include <cstring>
#endif

TaskScheduler::TaskScheduler() :
  numTasks(0)
{
	memset( &tasks, 0, sizeof(Task)*TaskScheduler::MaxTasks);
}

void TaskScheduler::add(Task* task) {
	if ( numTasks < TaskScheduler::MaxTasks ) {
		tasks[numTasks] = task;
		numTasks++;
	} else {
		//ERROR( F("More than ") << TaskScheduler::MaxTasks << 
		//	F(" tasks cannot be allocated.") );
	}
}

void TaskScheduler::run() {	
	unsigned long int nowU = HardwareInterface::getMicrosecondsSinceBoot();		
	unsigned long int now = HardwareInterface::getMillisecondsSinceBoot();		
	for (int i = 0; i < numTasks; i++) {		
		Task* thisTask = this->tasks[i];				
		if (thisTask->canRun(now)) {													
			thisTask->run(now);											
		} 		
	}	
}

void TaskScheduler::delayWithoutStoppingScheduler( unsigned long timeToWait )
{
    unsigned long startTime = HardwareInterface::getMillisecondsSinceBoot();
    unsigned long endTime = startTime + timeToWait;
    while ( HardwareInterface::getMillisecondsSinceBoot() < endTime ) {
        run();
    }
}