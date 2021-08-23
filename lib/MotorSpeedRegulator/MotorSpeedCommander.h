#ifndef _MOTORSPEEDCOMMANDER_H_
#define _MOTORSPEEDCOMMANDER_H_

#include <MotorSpeedRegulator.h>
#include <RecurringTaskBase.h>
#include <SerialStream.h>

class MotorSpeedCommander : public RecurringTaskBase
{
private:
    int posToMoveTo;
    int lapsToMove;
    long int maxSpeedToMove = 3500;
    unsigned long int timeToMove;
    unsigned long int timeToArrive;
    boolean hasArrivedAtPos = false;
    boolean onSameLapAsTarget = false;

    HomingEncoder *encoder;
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
        this->timeToArrive = _timeToArrive;
        this->posToMoveTo = _posToMoveTo;
        this->hasArrivedAtPos = false;

        long int pos = this->encoder->getPosComp();
        if (pos > this->posToMoveTo)
        {
            this->onSameLapAsTarget = false;
        }
        else
        {
            this->onSameLapAsTarget = true;
        }

        this->regulator->init();
        this->regulator->doHardBreak();
    }

    virtual void config(HomingEncoder *_encoder, MotorDriver *_driver,
                        MotorSpeedRegulator *_regulator)
    {
        this->encoder = _encoder;
        this->driver = _driver;
        this->regulator = _regulator;
    }

    virtual void run(unsigned long int _now)
    {
        //TODO: Rewrite this code. It is ugly as what.
        long int pos = this->encoder->getPosComp();
        if (!this->onSameLapAsTarget && pos < this->posToMoveTo)
        {
            //Detect that we have now moved to be on the same lap as the target
            this->onSameLapAsTarget = true; //We are now on the same lap as the target
        }
        long int clicksLeft = 0;
        if (!this->onSameLapAsTarget)
        {
            clicksLeft = encoder->positionPositiveDifference(this->posToMoveTo, pos);
        }
        else
        {
            clicksLeft = this->posToMoveTo - pos;
        }
        //End the rewrite here.

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

    HomingEncoder *getEncoder()
    {
        return this->encoder;
    }
};
#endif