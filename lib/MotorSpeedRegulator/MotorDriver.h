/*
 * MotorDriver.h
 *
 *  Created on: 24 okt. 2019
 *      Author: Daniel
 */

#ifndef MOTORDRIVER_H_
#define MOTORDRIVER_H_

#include <HardwarePins.h>
#include <MotorSpeedRegulatorInterfaces.h>

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cmath>
#endif

class MotorDriver: public MotorDriverInterface
{
private:
  int driverPinOne;
  int driverPinTwo;	  
  int driverPinPWM;
  int lastMotorPWM = 0;
  HardwarePinsInterface* pins;
  
public:	
  void config( int _driverPinOne, int _driverPinTwo, int _driverPinPWM, HardwarePinsInterface* _pins ) {
      driverPinOne = _driverPinOne;
      driverPinTwo = _driverPinTwo;
      driverPinPWM = _driverPinPWM;      

      pins = _pins;
		
      pins->configurePin ( this->driverPinOne, pins->OUTPUT );
      pins->configurePin ( this->driverPinTwo, pins->OUTPUT );
      pins->configurePin ( this->driverPinPWM, pins->OUTPUT );      

      this->setMotorPWM(0); 
  }
  
  virtual void setMotorPWM( int motorPWM ) {			  
    if ( motorPWM < 0 || 
      (motorPWM == 0 && lastMotorPWM > 0) ) //Actively break when setting the speed to 0
    {
      pins->setDigitalValueForPin( this->driverPinOne, pins->HIGH );
      pins->setDigitalValueForPin( this->driverPinTwo, pins->LOW );            
    } else if ( motorPWM > 0 || 
      (motorPWM == 0 && lastMotorPWM <= 0) ) //Actively break when setting the speed to 0. 
      //Also captures when motorPWM == 0 && lastMotorPWM == 0 for completeness sake.
    {        
      pins->setDigitalValueForPin( this->driverPinOne, pins->LOW );
      pins->setDigitalValueForPin( this->driverPinTwo, pins->HIGH );
    }
    
    pins->setAnalogValueForPin( this->driverPinPWM, abs(motorPWM) );    
    this->lastMotorPWM = motorPWM;
  }

  virtual int getMotorPWM()
  {
    return this->lastMotorPWM;
  }

};

#endif /* MOTORDRIVER_H_ */
