#ifndef _MAIN_H_
#define _MAIN_H_

#include "HomingEncoder.h"
#include <TaskScheduler.h>
#include <LevelLogger.h>
#include "MotorDriver.h"

#include "RecurringEncoderWrapper.h"
#include "getFreeMemory.h"
#include "RecurringTaskGroup.h"
#include "MotorSpeedRegulator.h"


TaskScheduler sched;

RecurringTaskGroup<16> recurring10ms( 10 );

HomingEncoder encoder;
EncoderWrapperComputeSpeedTask encoderWrapperComputeSpeed ( &encoder );
RecurringEncoderWrapperHoming<0> encoderWrapperHoming ( &encoder );

MotorDriver driver;

MotorSpeedRegulator regulator;

DataLogger dataLogger;

#endif