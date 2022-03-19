#pragma once

#include <RotationalPosition.h>

class MotorDriverInterface
{
    public:
        virtual void setMotorPWM( int16_t motorPWM ) = 0;
        virtual int16_t getMotorPWM() = 0;
};

struct MotorSpeedCommanderInterface
{
    virtual void setGoal( int16_t _goal, unsigned long _time ) = 0;
};

struct SpeedRegulatorInterface
{
    virtual void setSetPoint( int16_t _setPoint16_t ) = 0;
};

struct LegCommandControllerInterface
{
    struct LegCommand
    {
        int16_t targetPositionClicks = 0;
        long targetTimeMicros = 0;
    };

    virtual void receiveLegCommand(LegCommand) = 0;
};
