/*
 * MotorDriver.h
 *
 *  Created on: 24 okt. 2019
 *      Author: Daniel
 */

#ifndef MOTORDRIVER_H_
#define MOTORDRIVER_H_

#include <Arduino.h>

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
		
      pinMode(this->driverPinOne, OUTPUT);
      pinMode(this->driverPinTwo, OUTPUT);	      	
      pinMode(this->driverPinPWM, OUTPUT );

      this->setMotorPWM(0); 
  }
  
  void setMotorPWM( int motorPWM ) {			  
    if ( motorPWM < 0 || 
      (motorPWM == 0 && lastMotorPWM > 0) ) //Actively break when setting the speed to 0
    {
      digitalWrite ( this->driverPinTwo, LOW);
      digitalWrite ( this->driverPinOne, HIGH);			
    } else if ( motorPWM > 0 || 
      (motorPWM == 0 && lastMotorPWM < 0) ) //Actively break when setting the speed to 0
    {        
      digitalWrite ( this->driverPinTwo, HIGH);
      digitalWrite ( this->driverPinOne, LOW);		
    } else {
      digitalWrite ( this->driverPinTwo, LOW);
      digitalWrite ( this->driverPinOne, LOW);		
    }
    
    analogWrite ( this->driverPinPWM, abs(motorPWM) );
    this->lastMotorPWM = motorPWM;

    //Log << "Set motor pwm: " << motorPWM << endl;
  }
  
  float getCurrentInMilliVolt() {
    
    return -1; //Depricated
  }

  int getMotorPWM()
  {
    return this->lastMotorPWM;
  }

};

#endif /* MOTORDRIVER_H_ */
