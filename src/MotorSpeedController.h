#ifndef _SPEEDCONTROLLER_H_
#define _SPEEDCONTROLLER_H_

#include <Arduino.h>
#include <SerialStream.h>
#include <RecurringTask.h>
#include <HomingEncoder.h>

#include "MotorDriver.h" 

class MotorSpeedController: public RecurringTask
{
  private:
    MotorDriver* driver;
    HomingEncoder* encoder;
    int dest;
    int speed;
    bool running;

  public:
    MotorSpeedController( )
    {
      //Anything to init here?
    }

    void init ( unsigned int _rate, unsigned long int _now, MotorDriver* _driver, HomingEncoder* _encoder )
    {
      RecurringTask::init( _rate, _now );
      this->driver = _driver;
      this->encoder = _encoder;
    }

    void setDestinationPositionAndSpeed( int _dest, int _speed )
    {
      this->dest = _dest;

      int currPos = -encoder->getPosComp();
      while ( currPos > this->dest )
        this->dest += encoder->clicksPerRevolution();

      this->speed = _speed;

      this->running = true;

      driver->setMotorPWM( speed );
    }

    virtual void run ( unsigned long int now )
    {
      int currPos = -encoder->getPosComp();
      if ( currPos > this->dest )
      {
        this->driver->setMotorPWM( 0 );
        this->speed = 0;
      } else {
        Log << "Current position: " << currPos << endl;
      }
    }
};

#endif
