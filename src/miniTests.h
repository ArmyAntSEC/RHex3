#ifndef _MINITEST_H_
#define _MINITEST_H_

#include "HomingEncoder.h"
#include <SerialStream.h>
#include "MotorDriver.h"

void testDrift( HomingEncoder* encoder, MotorDriver *driver )
{
  for ( int i = 0; i < 10; i++ ) {    
    encoder->unHome();

    //int speed = -64;    
    int speed = -255;
    driver->setMotorPWM(speed);      
    int randNum = random(100);
    delay ( 200 + randNum  );
    
    //Measure the speed
    for ( int i = 0; i < 2; i++ ) {
      encoder->run(0);
      delay(10);
    }
    Log << "Speed: " << encoder->getSpeedCPMS() << endl;

    //Home the encoder
    while ( !encoder->isHomed() ) {    
      encoder->isr_homing<0>();       
    }
    
    //Measure the drift
    driver->setMotorPWM(0);
    delay(1000);  
    Log << "Stop position: " << encoder->getPosComp() << endl;
    Log << "Position at homing: " << encoder->getPosAtLastHome() << endl;    
  }
}

void testSpeedMesaurement( HomingEncoder* encoder, MotorDriver *driver )
{
    //int speed = -64;    
    int speed = -255;    
    driver->setMotorPWM(speed);      
    for ( int j = 1; j < 10; j++ ) {    
      for ( int i = 0; i < 50; i++ ) {
        encoder->run(0);
        delay(10);
      }
      Log << "Speed: " << encoder->getSpeedCPMS() << endl;
    }    
}

void testHomingPrecision( HomingEncoder* encoder, MotorDriver *driver )
{
    int speed = -64;    
    //int speed = -255;    
    driver->setMotorPWM(speed); 

    unsigned long int timeAtLastBreakerOffSwitch = 0;
    unsigned long int timeAtLastBreakerOnSwitch = 0;
    int lastBreakerVal = 0;    
    unsigned long int startTime = millis();

    while ( millis() < startTime + 10000 ) {
      int breakerVal = encoder->getBreakerVal();
      encoder->isr_homing<0>(); //Make sure homing code is run regularly      
      if ( breakerVal > 0 && breakerVal != lastBreakerVal ) {
        timeAtLastBreakerOnSwitch = micros();
        Log << "On(u): " << timeAtLastBreakerOnSwitch << 
          " Off(u): " << timeAtLastBreakerOffSwitch << 
          " Delta(ms): " << (timeAtLastBreakerOnSwitch - timeAtLastBreakerOffSwitch)/1000 << 
          " Position: " << encoder->getPosComp() << 
          " Direction: " << encoder->isMovingForward() << endl;        
      }
      if ( breakerVal == 0 && breakerVal != lastBreakerVal ) {
        timeAtLastBreakerOffSwitch = micros();
      }
      lastBreakerVal = breakerVal;
    }
    Log << "Done testing" << endl;
}

void testEncoderReliability( HomingEncoder* encoder, MotorDriver *driver )
{
  //int speed = -64;    
  int speed = -255;    
  driver->setMotorPWM(speed);   

  for ( int i = 0; i < 5; i++ ) {
    unsigned long int startTime = millis();
    while ( millis() < startTime + 1000 ) {
      encoder->isr_homing<0>(); //Make sure homing code is run regularly      
    }
    Log << "Pos since last homing: " << encoder->getLastLapLength() << 
    " Average Lap length: " << encoder->getAverageLapLength() << endl;
  }


}

#endif