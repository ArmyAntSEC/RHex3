#include <Arduino.h>
#include "LevelLogger.h"
#include "dataLogger.h"
#include <configureOneLeg.h>

#include "HomingEncoder.h"
#include <TaskScheduler.h>
#include <LevelLogger.h>

#include "getFreeMemory.h"
#include "RecurringTaskGroup.h"

#define UNCONNECTED_ANALOG A2

TaskScheduler sched;

RecurringTaskGroup<16> recurring10ms( 10 );



DataLogger dataLogger;

void setup()
{
  
  Serial.begin( 115200 );    
  DEBUG( F("Setup started") );  
  randomSeed(analogRead(UNCONNECTED_ANALOG));
  pinMode ( UNCONNECTED_ANALOG, INPUT );  
  
  //Initalize the driver
  driver.init( MOTOR_EN1, MOTOR_EN2, MOTOR_PWM, MOTOR_CS );  
  driver.setMotorPWM(0);   

  //Initialize the encoder and register the homing.
  encoder.init<0> ( ENCODER_1, ENCODER_2, OPTO, 0 );
  sched.add( &encoderWrapperHoming );
      
  //Initialize the regulator
  regulator.init(&encoder, &driver, 0.2, 0, 0.01, 1 );    
  recurring10ms.add( &regulator );    

  //Make sure we compute the encoder speed every 10ms
  recurring10ms.add( &encoderWrapperComputeSpeed );    

  //Initialize the data logger
  dataLogger.init(millis());  
  recurring10ms.add( &dataLogger );    

  //Now initialize the 10ms group    
  recurring10ms.init( millis() );  
  sched.add( &recurring10ms );
  
  dataLogger.sendHeaders(); 
  //ERROR( F("Setup done. Free RAM: ") << getFreeMemory() << " bytes." );  
}

void loop()
{  
  sched.run();
}



