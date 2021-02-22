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
  int currentPin;
  int driverPinPWM;
  
public:	
  void init( int _driverPinOne, int _driverPinTwo, int _driverPinPWM, 
	     int _currentPin ) {
      this->driverPinOne = _driverPinOne;
      this->driverPinTwo = _driverPinTwo;
      this->driverPinPWM = _driverPinPWM;
      
      this->currentPin = _currentPin;
		
      pinMode(this->driverPinOne, OUTPUT);
      pinMode(this->driverPinTwo, OUTPUT);	
      pinMode(this->currentPin, INPUT );	
      pinMode(this->driverPinPWM, OUTPUT );
  }
  
  void setMotorPWM( int motorPWM ) {			  
    if ( motorPWM <= 0 ) {
      digitalWrite ( this->driverPinTwo, LOW);
      digitalWrite ( this->driverPinOne, HIGH);			
    } else {
      digitalWrite ( this->driverPinTwo, HIGH);
      digitalWrite ( this->driverPinOne, LOW);		
    }
    
    analogWrite ( this->driverPinPWM, abs(motorPWM) );
  }
  
  float getCurrentInMilliVolt() {
    float voltageValue = analogRead( this->currentPin ) * 3.3 / 1024; //Read signal in Volts
    return voltageValue / 0.14; //Convert to A.
  }
};

#endif /* MOTORDRIVER_H_ */
