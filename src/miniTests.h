#ifndef _MINITEST_H_
#define _MINITEST_H_

#include "HomingEncoder.h"
#include <SerialStream.h>
#include "MotorDriver.h"

#define sgn(x) ((x) < 0 ? -1 : ((x) > 0 ? 1 : 0))

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

void testGoToPosition( HomingEncoder* encoder, MotorDriver *driver )
{  
  int targetSpeed = 7500;  

  int power = -64;    
  driver->setMotorPWM(power);   
  int randNum = random(100);
  delay ( 200 + randNum  );
  
  boolean done = false;
  unsigned long int nextRound = millis() + 10;  
  unsigned long int nextRound2 = millis() + 1000;  
  unsigned long int startTime = millis();
  while ( !done && millis() - startTime < 5000 ) {
    encoder->isr_homing<0>(); //Make sure homing code is run regularly        
    if ( millis() > nextRound ) {
      encoder->run(0);
      nextRound += 10;

    if ( millis() > nextRound2 ) {
      nextRound2 += 1000;
      if ( encoder->isHomed() && encoder->getSpeedCPMS() > 0 ) {
          long int speed = encoder->getSpeedCPMS();        
          int newPower = power * (float)targetSpeed / (float)speed;
          if ( abs(newPower) > 255 ) {
            newPower = 255*sgn(newPower);
          }

          driver->setMotorPWM(power);   
          Log << "Speed: " << speed <<
            " New Power: " << newPower << 
            " Old Power: " << power << endl;
          power = newPower;
        }      
      }
    }
  }    
}

#endif