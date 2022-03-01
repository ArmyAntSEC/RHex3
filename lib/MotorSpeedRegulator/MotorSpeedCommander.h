#pragma once

#include <MotorSpeedRegulatorInterfaces.h>

class MotorSpeedCommander: public MotorSpeedCommanderInterface
{
private:
    RotationalPositionProvider* currentRotPos;
    SpeedRegulatorInterface* speedRegulator;
    RotationalPosition positionGoal;
    unsigned long timeGoal;
    int maxSpeedCPS;

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
    void configure( RotationalPositionProvider* _rotPos, SpeedRegulatorInterface* _speedRegulator, int _maxSpeedCPS )
    {
        currentRotPos = _rotPos;
        speedRegulator = _speedRegulator;
        maxSpeedCPS = _maxSpeedCPS;
    }

    virtual void setGoal( RotationalPosition _pos, unsigned long _time )
    {
        positionGoal = _pos;
        timeGoal = _time;
    }    

    long computeTargetSpeedCPS( long timeLeftMicros, long clicksLeft )
    {
        long int targetSpeedCPS = targetSpeedCPS = (1000L * clicksLeft) / timeLeftMicros;

        return cappedSpeedCPS( targetSpeedCPS );
    }
};