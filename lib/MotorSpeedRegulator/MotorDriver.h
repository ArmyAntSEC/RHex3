/*
 * MotorDriver.h
 *
 *  Created on: 24 okt. 2019
 *      Author: Daniel
 */

#ifndef MOTORDRIVER_H_
#define MOTORDRIVER_H_

#include <HardwareInterface.h>

class MotorDriver {
private:
  int driverPinOne;
  int driverPinTwo;	  
  int driverPinPWM;
  int lastMotorPWM = 0;
  
public:	
  void config( int _driverPinOne, int _driverPinTwo, int _driverPinPWM ) {
      this->driverPinOne = _driverPinOne;
      this->driverPinTwo = _driverPinTwo;
      this->driverPinPWM = _driverPinPWM;      
		
      HardwareInterface::configurePin ( this->driverPinOne, HardwareInterface::OUTPUT );
      HardwareInterface::configurePin ( this->driverPinTwo, HardwareInterface::OUTPUT );
      HardwareInterface::configurePin ( this->driverPinPWM, HardwareInterface::OUTPUT );

      this->setMotorPWM(0); 
  }
  
  void setMotorPWM( int motorPWM ) {			  
    if ( motorPWM < 0 || 
      (motorPWM == 0 && lastMotorPWM > 0) ) //Actively break when setting the speed to 0
    {
      HardwareInterface::setDigitalValueForPin( this->driverPinOne, HardwareInterface::HIGH );
      HardwareInterface::setDigitalValueForPin( this->driverPinTwo, HardwareInterface::LOW );            
    } else if ( motorPWM > 0 || 
      (motorPWM == 0 && lastMotorPWM < 0) ) //Actively break when setting the speed to 0
    {        
      HardwareInterface::setDigitalValueForPin( this->driverPinOne, HardwareInterface::LOW );
      HardwareInterface::setDigitalValueForPin( this->driverPinTwo, HardwareInterface::HIGH );
    } else {
      HardwareInterface::setDigitalValueForPin( this->driverPinOne, HardwareInterface::LOW );
      HardwareInterface::setDigitalValueForPin( this->driverPinTwo, HardwareInterface::LOW );
    }
    
    HardwareInterface::setAnalogValueForPin( this->driverPinPWM, abs(motorPWM) );    
    this->lastMotorPWM = motorPWM;
  }

  int getMotorPWM()
  {
    return this->lastMotorPWM;
  }

};

#endif /* MOTORDRIVER_H_ */
