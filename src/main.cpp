#include <Arduino.h>
#include "LevelLogger.h"
#include "dataLogger.h"
#include "main.h"


#define MOTOR_EN1 4
#define MOTOR_EN2 5
#define MOTOR_CS A0
#define ENCODER_2 2
#define ENCODER_1 3
#define OPTO 7
#define MOTOR_PWM 6
#define UNCONNECTED_ANALOG A2

#ifdef HIGH_LEVEL
  #include "mainHighLevel.h"
#else
  #include "miniTests/mainLowLevel.h"
#endif

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
  recurring10ms.add( &regulator );    

  //Make sure we compute the encoder speed every 10ms
  recurring10ms.add( &encoderWrapperComputeSpeed );    

  //Initialize the data logger
  dataLogger.init(millis());  
  recurring10ms.add( &dataLogger );    

  //Now initialize the 10ms group    
  recurring10ms.init( millis() );  
  sched.add( &recurring10ms );
  
  
  //Add any remaining setup needed for the current configuration.
  setupDetails();

  dataLogger.sendHeaders(); 
  //ERROR( F("Setup done. Free RAM: ") << getFreeMemory() << " bytes." );  
}

void loop()
{  
  sched.run();
}



