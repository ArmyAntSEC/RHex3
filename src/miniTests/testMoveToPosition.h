#ifndef _TESTMOVETOPOSITION_H_
#define _TESTMOVETOPOSITION_H_

#include "RemoteRoutine.h"

class TestMoveToPosition: public RemoteRoutine
{
    private:
    public:    
};
/*
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
  encoder->run(0); //Init the speed measurement
  while ( millis() - startTime < 2000 )
  {
    encoder->isr_homing<0>(); //Make sure homing code is run regularly      
    if ( millis() > nextTime ) {
      encoder->run(0);
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
        speedLog[loopCount] = encoder->getSpeedCPMS();
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
*/

#endif