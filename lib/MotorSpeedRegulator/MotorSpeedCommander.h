#pragma once

#include <MotorSpeedRegulatorInterfaces.h>
#include <RunnableInterface.h>

class MotorSpeedCommander: public MotorSpeedCommanderInterface, RunnableInterface
{
private:
    RotationalPositionProvider* currentRotPos;
    SpeedRegulatorInterface* speedRegulator;
    RotationalPosition positionGoal;
    unsigned long timeGoalMicros;
    int maxSpeedCPS = 5000;
    bool arrived = false;

    long cappedSpeedCPS( long speedCPS )
    {
        if ( speedCPS < 0 || speedCPS > maxSpeedCPS )
        {
            return maxSpeedCPS;
        } else {
            return speedCPS;
        }
    }

public:
    void config( RotationalPositionProvider* _rotPos, SpeedRegulatorInterface* _speedRegulator, int _maxSpeedCPS )
    {
        currentRotPos = _rotPos;
        speedRegulator = _speedRegulator;
        maxSpeedCPS = _maxSpeedCPS;
    }

    virtual void setGoal( RotationalPosition _pos, unsigned long _time )
    {
        positionGoal = _pos;
        timeGoalMicros = _time;
    }    

    long computeTargetSpeedCPS( long timeLeftMicros, long clicksLeft )
    {        
        long clicksLeftScaled = 1000L * clicksLeft;        
        long targetSpeedCPS = clicksLeftScaled / timeLeftMicros;
        long targetSpeedCPSCapped = cappedSpeedCPS( targetSpeedCPS );
        
        return targetSpeedCPSCapped;
    }

    virtual void run(unsigned long int nowMicros)
    {
        long int clicksLeft = positionGoal.getLinearPosition() - currentRotPos->getLinearPosition();
        if ( clicksLeft < 0 )
        {
            arrived = true;
            speedRegulator->setSetPoint( 0 );            
        } else {
            arrived = false;
            long timeLeft = timeGoalMicros - nowMicros;
            long targetSpeed = computeTargetSpeedCPS( timeLeft, clicksLeft ); 
            speedRegulator->setSetPoint( targetSpeed );            
        }        
    }

    bool hasArrived()
    {
        //TODO: Make sure that this value is reset when new position is sent.
        return arrived;
    }
};
