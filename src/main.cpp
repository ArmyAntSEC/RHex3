#include <Arduino.h>

#define MOTOR_EN1 4
#define MOTOR_EN2 5
#define MOTOR_CS A0
#define ENCODER_2 2
#define ENCODER_1 3
#define OPTO 7
#define MOTOR_PWM 6
#define UNCONNECTED_ANALOG A2

#include "HomingEncoder.h"
#include <TaskScheduler.h>
#include <LevelLogger.h>
#include "MotorDriver.h"
#include "miniTests/SimpleMoveTest.h"
#include "CommandAndControll.h"
#include "RecurringEncoderWrapper.h"
#include "miniTests/dataLogger.h"
#include "getFreeMemory.h"
#include "RecurringTaskGroup.h"

TaskScheduler sched;

RecurringTaskGroup<16> recurring10ms( 10 );

HomingEncoder encoder;
EncoderWrapperComputeSpeedTask encoderWrapperComputeSpeed ( &encoder );
RecurringEncoderWrapperHoming<0> encoderWrapperHoming ( &encoder );

MotorDriver driver;

SimpleMoveTest simpleMoveTest( &encoder, &driver );

CommandAndControll ctr;

DataLogger dataLogger;


void setup()
{
  
  Serial.begin( 115200 );    
  DEBUG( F("Setup started") );  
  randomSeed(analogRead(UNCONNECTED_ANALOG));
  
  pinMode ( MOTOR_EN1, OUTPUT );
  pinMode ( MOTOR_EN2, OUTPUT );
  pinMode ( MOTOR_PWM, OUTPUT );
  pinMode ( UNCONNECTED_ANALOG, INPUT );  

  ctr.init();

  driver.init( MOTOR_EN1, MOTOR_EN2, MOTOR_PWM, MOTOR_CS );  
  driver.setMotorPWM(0);   

  encoder.init<0> ( ENCODER_1, ENCODER_2, OPTO, 0 );
  
  sched.add( &encoderWrapperHoming );
  recurring10ms.add( &encoderWrapperComputeSpeed );  
  
  recurring10ms.add(&simpleMoveTest);      
    
  ctr.registerRemoteRoutine(&simpleMoveTest,0);  

  recurring10ms.add( &ctr );

  sched.add( &recurring10ms );
  recurring10ms.init( millis() );  
  

  ERROR( F("Setup done. Free RAM: ") << getFreeMemory() << " bytes." );  
}

void loop()
{  
  sched.run();
}



