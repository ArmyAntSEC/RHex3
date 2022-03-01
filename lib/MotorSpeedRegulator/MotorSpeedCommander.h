#pragma once

#include <MotorSpeedRegulatorInterfaces.h>

class MotorSpeedCommander: public MotorSpeedCommanderInterface
{
private:
    RotationalPosition* positionGoal;
    unsigned long timeGoal;

public:
    virtual void setGoal( RotationalPosition* _pos, unsigned long _time )
    {
        positionGoal = _pos;
        timeGoal = _time;
    }
};