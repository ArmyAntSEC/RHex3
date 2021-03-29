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

#include "miniTests/dataLogger.h"
#include "miniTests/TestSimpleMove.h"
#include "miniTests/TestMoveAtGivenSpeed.h"
#include "miniTests/testMoveToPosition.h"

#include "CommandAndControll.h"
#include "RecurringEncoderWrapper.h"
#include "getFreeMemory.h"
#include "RecurringTaskGroup.h"
#include "MotorSpeedRegulator.h"

TaskScheduler sched;
DataLogger dataLogger;

RecurringTaskGroup<16> recurring10ms( 10 );

HomingEncoder encoder;
EncoderWrapperComputeSpeedTask encoderWrapperComputeSpeed ( &encoder );
RecurringEncoderWrapperHoming<0> encoderWrapperHoming ( &encoder );

MotorDriver driver;

MotorSpeedRegulator regulator;

SimpleMoveTest simpleMoveTest( &encoder, &driver, &dataLogger );
TestMoveAtGivenSpeed testMoveAtGivenSpeed( &encoder, &driver, &regulator, &dataLogger );
TestMoveToPosition testMoveToPos (  &encoder, &driver, &regulator, &dataLogger );

CommandAndControll ctr( &dataLogger );

void setup()
{
  
  Serial.begin( 115200 );    
  DEBUG( F("Setup started") );  
  randomSeed(analogRead(UNCONNECTED_ANALOG));
  
  pinMode ( MOTOR_EN1, OUTPUT );
  pinMode ( MOTOR_EN2, OUTPUT );
  pinMode ( MOTOR_PWM, OUTPUT );
  pinMode ( UNCONNECTED_ANALOG, INPUT );  

  //Initalize the driver
  driver.init( MOTOR_EN1, MOTOR_EN2, MOTOR_PWM, MOTOR_CS );  
  driver.setMotorPWM(0);   

  //Initialize the encoder and register the homing.
  encoder.init<0> ( ENCODER_1, ENCODER_2, OPTO, 0 );
  sched.add( &encoderWrapperHoming );
      
  //Initialize the regulator
  regulator.init(&encoder, &driver, 0.2, 0, 0.01, 1 );

  //Initialize the Command and Controll
  DEBUG( F("CTR Init") );
  ctr.init();
  ctr.registerRemoteRoutine(&simpleMoveTest,0);  
  ctr.registerRemoteRoutine(&testMoveAtGivenSpeed,1);  
  ctr.registerRemoteRoutine(&testMoveToPos,2);  

  //Now configure the 10ms group
  recurring10ms.add( &regulator );    
  recurring10ms.add( &encoderWrapperComputeSpeed );    
  
  recurring10ms.add( &simpleMoveTest );      
  recurring10ms.add( &testMoveAtGivenSpeed );      
  recurring10ms.add( &testMoveToPos );      
  
  recurring10ms.add( &ctr );
  recurring10ms.add( &dataLogger ); //Run the data logger last.
  
  
  recurring10ms.init( millis() );  
  sched.add( &recurring10ms );
    

  ERROR( F("Setup done. Free RAM: ") << getFreeMemory() << " bytes." );  
  //dataLogger.sendHeaders();
}

void loop()
{  
  sched.run();
}



