#ifndef _MINITEST_H_
#define _MINITEST_H_

#include "HomingEncoder.h"
#include <SerialStream.h>
#include "MotorDriver.h"

void testEncoderReliability( HomingEncoder* encoder, MotorDriver *driver )
{
  for ( int i = 0; i < 5; i++ ) {    
    encoder->unHome();

    int speed = -64;    
    driver->setMotorPWM(speed);      
    int randNum = random(100);
    delay ( 200 + randNum  );
    encoder->run( micros() );
    delay(1);
    encoder->run( micros() );
    Log << "Speed: " << encoder->readSpeed() << endl;
    while ( !encoder->isHomed() ) {    
      encoder->isr_homing<0>(); 
      delay(10);   
    }
    driver->setMotorPWM(0);
    delay(1000);  
    Log << "Stop position: " << encoder->readCompensatedPos() << endl;
    Log << "Position at homing: " << encoder->getPosAtLastHome() << endl;    
  }
}

#endif