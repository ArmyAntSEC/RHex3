#include <Arduino.h>
#include "LevelLogger.h"
#include "dataLogger.h"
#include <configureOneLeg.h>

#include "HomingEncoder.h"
#include <TaskScheduler.h>
#include <LevelLogger.h>

#include "getFreeMemory.h"
#include "RecurringTaskGroup.h"

#include <configureOneLeg.h>

#define UNCONNECTED_ANALOG A2

TaskScheduler sched;

RecurringTaskGroup<16> recurring10ms( 10 );



DataLogger dataLogger;

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}    
  Serial.println("Hello World!");

  DEBUG( F("Setup started") );  
  
  randomSeed(analogRead(UNCONNECTED_ANALOG));
  pinMode ( UNCONNECTED_ANALOG, INPUT );  
  
  initOneLeg();
  
  /*
  //Start the motors just to test
  driver.setMotorPWM(64);
  delay(1000);
  driver.setMotorPWM(0);
  */

  //Test the encoder
  while ( true ) {
    Serial.print( "Breaker: " );
    Serial.print( encoder.isHomed() );
    Serial.print( " Encoder: " );
    Serial.print( encoder.getPosition().getSerialPosition() );
    Serial.println();
    delay(100);
  }


  /*
  //Initalize the driver
  driver.config( MOTOR_EN1, MOTOR_EN2, MOTOR_PWM, MOTOR_CS );  
  driver.setMotorPWM(0);   
  
  //Initialize the encoder and register the homing.
  encoder.config<0> ( ENCODER_1, ENCODER_2, OPTO, 0 );
  sched.add( &encoderWrapperHoming );
      
  //Initialize the regulator
  regulator.config(&encoder, &driver, 0.2, 0, 0.01, 1 );    
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
  */
}

void loop()
{  
  //sched.run();
}



