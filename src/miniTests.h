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
    int speed = -64;    
    //int speed = -255;    
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

void testSpeedPDController( HomingEncoder* encoder, MotorDriver *driver )
{  
  //int speedLog[500];
  //int powerLog[500];
  int loopCount = 0;

  float P = 10;
  float D = 0.0;
  int Input = 0;
  long int Output = 0;
  int SetPoint = 5000;
  int lastInput = 0;
    

  long int ErrorVariance = 0;

  unsigned long int startTime = millis();
  unsigned long int nextTime = millis() + 10;  
  while ( millis() - startTime < 3000 )
  {
    encoder->isr_homing<0>(); //Make sure homing code is run regularly      
    if ( millis() > nextTime ) {
      encoder->run(0);
      nextTime += 10;
      
      Input = encoder->getSpeedCPMS();
      int Error = SetPoint - Input;
      int dInput = Input - lastInput;
      lastInput = Input;

      Output = P*Error + D*dInput;
      if ( Output > 255  ) Output = 255;
      if ( Output < 0 ) Output = 1;      
            
      driver->setMotorPWM(Output);   

      ErrorVariance += (long int)Error*(long int)Error;

      //speedLog[loopCount] = Input;
      //powerLog[loopCount] = Output;

      loopCount++;
    }
  }
  Log << "Last speed: " << Input << endl;    
  Log << "Error Std: " << sqrt( ErrorVariance )/loopCount << endl;
  Log << "Loop count: " << loopCount;
    
  /*for ( int i = 0; i < 1; i++ ) {
    Log << speedLog[i] << ", " << powerLog[i] << endl;
    delay(10);
  } */ 
}

#endif