#ifndef _MINITEST_H_
#define _MINITEST_H_

#include "HomingEncoder.h"
#include <SerialStream.h>
#include "MotorDriver.h"
#include "MotorSpeedRegulator.h"

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
      encoder->run();
      delay(10);
    }
    Log << "Speed: " << encoder->getSpeedCPS() << endl;

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
        encoder->run();
        delay(10);
      }
      Log << "Speed: " << encoder->getSpeedCPS() << endl;
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

void testSpeedPDController( HomingEncoder* encoder, MotorDriver *driver, float parameters[4] )
{  
  int speedLog[200];
  int powerLog[200];
  int loopCount = 0;

  float P = parameters[0]; 
  float D = parameters[1];
  float filter = parameters[2];

  int Input = 0;
  long int Output = 0;
  long int OutputFiltered = 0;
  int SetPoint = parameters[3];
  int lastInput = 0;
    
  //Make sure that we are homed before we start.
  encoder->forceHomed();

  long int ErrorVariance = 0;
  unsigned long int startTime = millis();
  unsigned long int nextTime = millis() + 10;  
  while ( millis() - startTime < 2000 )
  {
    encoder->isr_homing<0>(); //Make sure homing code is run regularly      
    if ( millis() > nextTime ) {
      encoder->run();
      nextTime += 10;
      
      Input = encoder->getSpeedCPS();
      int Error = SetPoint - Input;
      int dInput = Input - lastInput;
      lastInput = Input;

      Output = P*Error + D*dInput;
      if ( Output > 255  ) Output = 255;
      if ( Output < 0 ) Output = 1;      
            
      OutputFiltered = (Output + OutputFiltered*(filter-1))/filter;
      driver->setMotorPWM(OutputFiltered);   

      ErrorVariance += (long int)Error*(long int)Error;

      speedLog[loopCount] = Input;
      powerLog[loopCount] = OutputFiltered;

      loopCount++;

      if ( millis() - startTime > 1000 ) { 
        //After 1 second, set the speed to half.
        SetPoint = parameters[3]/2;
      }      
    }
  }
  //Log << "Last speed: " << Input << endl;    
  //Log << "Error Std: " << sqrt( ErrorVariance )/loopCount << endl;
  //Log << "Loop count: " << loopCount << endl;
  
  Log << "Speed, Power" << endl;
  for ( int i = 0; i < loopCount-1; i++ ) {
    Log << speedLog[i] << ", " << powerLog[i] << endl;
  }
}

void testSpeedPDControllerClass( HomingEncoder* encoder, MotorDriver *driver, float parameters[4] )
{  
  int speedLog[200];
  int powerLog[200];
  int posLog[200];
  int loopCount = 0;
  
  MotorSpeedRegulator regulator;
  regulator.init( encoder, driver, parameters[0], parameters[1], parameters[2] );
  regulator.setSetPoint( parameters[3] );
  
  //Home the encoder
  driver->setMotorPWM( 92 );
  while ( !encoder->isHomed() ) {    
    encoder->isr_homing<0>(); //Make sure homing code is run regularly      
  }
  driver->setMotorPWM( 0 );
  delay(500); //Make sure the motor has stopped.    
  
  unsigned long int startTime = millis();
  unsigned long int nextTime = millis() + 10;  
  encoder->run(); //Init the speed measurement
  while ( millis() - startTime < 2000 )
  {
    encoder->isr_homing<0>(); //Make sure homing code is run regularly      
    if ( millis() > nextTime ) {
      encoder->run();
      nextTime += 10;
      
      regulator.run();

      speedLog[loopCount] = regulator.getInput();
      powerLog[loopCount] = regulator.getFilteredOutput();
      posLog[loopCount] = encoder->getPosComp();        

      loopCount++;

      if ( millis() - startTime > 1000 ) { 
        //After 1 second, set the speed to half.
        regulator.setSetPoint( parameters[3]/2 );
      }      
    }
  }

  Log << "Speed, Power, Position" << endl;
  for ( int i = 0; i < loopCount-1; i++ ) {
    Log << speedLog[i] << ", " << powerLog[i] << ", " << posLog[i] << endl;
  }
}

void testGoToPosition( HomingEncoder* encoder, MotorDriver *driver, float parameters[4] )
{  
  int speedLog[200];
  int powerLog[200];
  int posLog[200];
  int loopCount = 0;
  bool done = false;
  
  MotorSpeedRegulator regulator;
  regulator.init( encoder, driver, parameters[0], parameters[1], parameters[2] );
  regulator.setSetPoint( parameters[3] );
  
  //Home the encoder
  driver->setMotorPWM( 96 );
  while ( !encoder->isHomed() ) {    
    encoder->isr_homing<0>(); //Make sure homing code is run regularly      
  }
  driver->setMotorPWM( 0 );
  
  delay(500); //Make sure the motor has stopped.  
  
  unsigned long int startTime = millis();
  unsigned long int nextTime = millis() + 10;    
  encoder->run(); //Init the speed measurement
  while ( millis() - startTime < 2000 )
  {
    encoder->isr_homing<0>(); //Make sure homing code is run regularly      
    if ( millis() > nextTime ) {
      encoder->run();
      nextTime += 10;
      
      if ( encoder->getPosComp() > 3000 ) {
        done = true;
      }
      if ( !done ) {         
        regulator.run();
        speedLog[loopCount] = regulator.getInput();
        powerLog[loopCount] = regulator.getFilteredOutput();
        posLog[loopCount] = encoder->getPosComp();        
      } else {
        driver->setMotorPWM( 0 );
        speedLog[loopCount] = encoder->getSpeedCPS();
        powerLog[loopCount] = 0;
        posLog[loopCount] = encoder->getPosComp();        
      }

      loopCount++;

    }
  }

  Log << "Speed, Power, Position" << endl;
  for ( int i = 0; i < loopCount-1; i++ ) {
    Log << speedLog[i] << ", " << powerLog[i] << ", " << posLog[i] << endl;
  }
}
#endif