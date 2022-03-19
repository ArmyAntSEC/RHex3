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

struct MotorSpeedCommanderInterface
{
    struct LegCommand
    {
        int16_t targetPositionClicks;
        int32_t targetTimeMicros;
        LegCommand( int16_t _clicks = 0, int32_t _timeMicros = 0 ): targetPositionClicks(_clicks), targetTimeMicros(_timeMicros)
        {}
    };

    virtual void setGoal(LegCommand) = 0;
};
