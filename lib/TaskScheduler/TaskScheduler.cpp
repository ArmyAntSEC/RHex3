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

void TaskScheduler::run(unsigned long now) {			
	for (int i = 0; i < numTasks; i++) {		
		Task* thisTask = this->tasks[i];				
		if (thisTask->canRun(now)) {													
			thisTask->run(now);											
		} 		
	}	
}

void TaskScheduler::delayWithoutStoppingScheduler( unsigned long timeToWait )
{
    unsigned long now = HardwareInterface::getMillisecondsSinceBoot();
    unsigned long endTime = now + timeToWait;
    while ( now < endTime ) {
        run(now);
		now = HardwareInterface::getMillisecondsSinceBoot();
    }
}