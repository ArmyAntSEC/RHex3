#ifndef _MOTORSPEEDCOMMANDER_H_
#define _MOTORSPEEDCOMMANDER_H_

#include <MotorSpeedRegulator.h>
#include <RecurringTaskBase.h>
#include <SerialStream.h>

class MotorSpeedCommander : public RecurringTaskBase
{
private:
    RotationPositionWithLaps posToMoveTo;
    int lapsToMove;
    long int maxSpeedToMove = 3500;
    unsigned long int timeToMove;
    unsigned long int timeToArrive;
    boolean hasArrivedAtPos = false;    

    HomingEncoderState *encoder;
    MotorDriver *driver;

    MotorSpeedRegulator *regulator;

public:
    int getMaxSpeed() { return this->maxSpeedToMove; }
    
    long int computeTargetSpeedTakingNegativeTimeIntoAccount( long int timeLeft, long int clicksLeft )
    {
        long int targetSpeed = (1000L * clicksLeft) / timeLeft;
        if ( timeLeft < 0 ) {
            targetSpeed = this->maxSpeedToMove;
        }
        return targetSpeed;
    }
    
    long int capTargetSpeed( long int targetSpeed )
    {
        if ( targetSpeed > this->maxSpeedToMove )
        {            
            targetSpeed = this->maxSpeedToMove;        
        } else if ( targetSpeed <= 0 ) {
            targetSpeed = 1; //If we are asked for a negative speed, we simply coast.
        }
        return targetSpeed;
    }

    int computeTargetSpeed(long int timeLeft, long int clicksLeft)
    {        
        long int targetSpeed = computeTargetSpeedTakingNegativeTimeIntoAccount( timeLeft, clicksLeft );
        
        targetSpeed = capTargetSpeed( targetSpeed );
        
        return targetSpeed;
    }

    virtual void init(unsigned long int _timeToArrive,
                      int _posToMoveTo)
    {
        RecurringTaskBase::init();
        RotationPositionWithLaps currentPosition = this->encoder->getPosition();

        this->timeToArrive = _timeToArrive;
        this->posToMoveTo = currentPosition;
        this->posToMoveTo.moveForwardTo( _posToMoveTo );

        this->hasArrivedAtPos = false;
    
        this->regulator->init();
        this->regulator->useHardBreaks();
    }

    virtual void config(HomingEncoderState *_encoder, MotorDriver *_driver,
                        MotorSpeedRegulator *_regulator)
    {
        this->encoder = _encoder;
        this->driver = _driver;
        this->regulator = _regulator;
    }

    virtual void run(unsigned long int _now)
    {        
        RotationPositionWithLaps pos = this->encoder->getPosition();        
        long int clicksLeft = this->posToMoveTo.getDifferenceInClicks( &pos );

        if (clicksLeft < 0)
        {
            clicksLeft = 0;
            this->hasArrivedAtPos = true;
        }

        long int targetSpeedClamped = this->computeTargetSpeed(
            this->timeToArrive - _now, clicksLeft);

        if (!this->hasArrivedAtPos)
        {
            this->regulator->setSetPoint(targetSpeedClamped);
        }
    }

    boolean hasArrived()
    {
        return this->hasArrivedAtPos;
    }

    virtual void stop()
    {
        RecurringTaskBase::stop();
        regulator->stop();
    }

    HomingEncoderState *getEncoder()
    {
        return this->encoder;
    }
};
#endif