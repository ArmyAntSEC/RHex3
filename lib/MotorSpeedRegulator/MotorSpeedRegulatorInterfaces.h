#pragma once

#include <RotationalPosition.h>

class MotorDriverInterface
{
    public:
        virtual void setMotorPWM( int motorPWM ) = 0;
        virtual int getMotorPWM() = 0;
};

struct MotorSpeedCommanderInterface
{
    virtual void setGoal( int _goal, unsigned long _time ) = 0;
};

struct SpeedRegulatorInterface
{
    virtual void setSetPoint( int _setPoint ) = 0;
};
