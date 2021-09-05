#pragma once

#ifdef ARDUINO
#include <Arduino.h>
#endif

class HomingEncoderHardwareInterface
{
  public:
  enum TriggerModes { _FALLING, _CHANGE };

  void configurePins ( unsigned int encoderPin1, unsigned int encoderPin2, 
    unsigned int homingPin )
  {
    #ifdef ARDUINO
    pinMode(encoderPin1, INPUT_PULLUP);
    pinMode(encoderPin2, INPUT_PULLUP);
    pinMode(homingPin, INPUT_PULLUP);
    #endif //Ignore this if we have no hardware.
  }

  void attachAnInterrupt(unsigned int pin, void(*isr)(), TriggerModes mode )
  {            
    #ifdef ARDUINO
    PinStatus triggerMode = CHANGE;
    switch ( mode ) {
      case _FALLING:
        triggerMode = FALLING;
        break;
      case _CHANGE:  
        triggerMode = CHANGE;
        break;        
    }
    attachInterrupt(digitalPinToInterrupt(pin), isr, triggerMode);    
    #endif //Ignore this if we have no hardware
  }

  void disableInterrupts()
  {
    #ifdef ARDUINO
    noInterrupts();
    #endif
  }

  void enableInterrupts()
  {
    #ifdef ARDUINO
    interrupts();
    #endif
  }

  unsigned long int getMicrosecondsSinceBoot()
  {
    #ifdef ARDUINO
    return micros();
    #else
    return 0;
    #endif
  }

  int getDigitalValueFromPin(int pin )
  {
    #ifdef ARDUINO
    return digitalRead( pin );
    #else
    return 0;
    #endif
  }
};
