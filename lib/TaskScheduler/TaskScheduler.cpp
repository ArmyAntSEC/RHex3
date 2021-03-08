/*
 * TaskScheduler.cpp
 *
 *  Created on: 8 juli 2020
 *      Author: Daniel
 */

#include "TaskScheduler.h"

#include <LevelLogger.h>

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
		ERROR( F("More than ") << TaskScheduler::MaxTasks << 
			F(" tasks cannot be allocated.") );
	}
}

void TaskScheduler::run() {	
	if ( millis() > 1100 )  {
		return;
	}
	
	unsigned long int nowU = micros();		
	unsigned long int now = millis();	
	for (int i = 0; i < numTasks; i++) {
		DEBUG(F("Checking task ") << i << " of " << numTasks << F(" at time ") << now );
		Task* thisTask = this->tasks[i];		
		if (thisTask->canRun(now)) {							
			thisTask->run(now);											
		} 
		DEBUG(F("Done with task ") << i << " of " << numTasks << F(" at time ") << now );				
	}
	DEBUG(F("----- Done with run") );
	unsigned long totalTime = micros() - nowU;
	if ( this->averageLoopTime == 0 ) {
		this->averageLoopTime = totalTime;
	} else {
		this->averageLoopTime = 
			(this->averageLoopTime*(TaskScheduler::filterFactor-1) +
			totalTime) / TaskScheduler::filterFactor;
	}
	if ( now > this->nextRunTime ) {
		ERROR(F("Loop time: ") << this->averageLoopTime << "\u03BCs." );
		this->nextRunTime += 1000;
	}
}
