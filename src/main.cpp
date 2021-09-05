#ifdef ARDUINO
#include <Arduino.h>
#include "dataLogger.h"
#include <configureOneLeg.h>

#include "HomingEncoder.h"
#include <TaskScheduler.h>

#include "getFreeMemory.h"
#include "RecurringTaskGroup.h"

#include <configureOneLeg.h>

#define UNCONNECTED_ANALOG A2

TaskScheduler sched;

RecurringTaskGroup<16> recurring10ms( 10 );



//DataLogger dataLogger;

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}    
  Serial.println("Hello World!");

  //randomSeed(analogRead(UNCONNECTED_ANALOG));
  //pinMode ( UNCONNECTED_ANALOG, INPUT );  
  
  initOneLeg();

  driver.setMotorPWM(255);

  //Test the encoder
  long endTime = millis() + 10000;
  
  pinMode( 13, OUTPUT );

  while ( endTime > millis() ) {
    
    Log << "[" << millis() << "] Breaker: " << encoder->getHomingPinValue() << " is homed: " << encoder->is_homed;
    Log << " Encoder: " << encoder->getPosition().getSerialPosition() << " Raw pos: " << encoder->raw_position;
    Log << endl;
    
    digitalWrite( 13, encoder->getHomingPinValue() ); //Blink the onboard LED as the homing breaker moves.  
    
    delay(10);        
  }

  driver.setMotorPWM(0);
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

#endif

int main()
{

}

