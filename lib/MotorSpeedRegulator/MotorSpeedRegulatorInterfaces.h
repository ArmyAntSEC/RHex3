#pragma once

#include <RotationalPosition.h>

class MotorDriverInterface
{
    public:
        virtual void setMotorPWM( int16_t motorPWM ) = 0;
        virtual int16_t getMotorPWM() = 0;
};

struct SpeedRegulatorInterface
{
    virtual void setSetPoint( int16_t _setPoint16_t ) = 0;
};

struct MotorCommanderGoal
{
    int16_t targetPositionClicks;
    int32_t targetRelativeTimeMicros;
    MotorCommanderGoal( int16_t _targetPositionClicks = 0, int32_t _targetRelativeTimeMicros = 0 ):
        targetPositionClicks(_targetPositionClicks), targetRelativeTimeMicros(_targetRelativeTimeMicros)
    {}
};

struct MotorSpeedCommanderInterface
{    
    virtual void setGoal(MotorCommanderGoal command, int32_t nowMicros ) = 0;
};

