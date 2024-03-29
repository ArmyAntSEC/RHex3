#pragma once

#include <RotationalPosition.h>

class MotorDriverInterface
{
public:
    virtual void setMotorPWM(int16_t motorPWM) = 0;
    virtual int16_t getMotorPWM() = 0;
};

struct SpeedRegulatorInterface
{
    virtual void setSetPoint(int16_t _setPoint16_t) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
};

struct MotorCommanderGoal
{
    int32_t targetPositionClicks;
    int32_t targetRelativeTimeMicros;
    MotorCommanderGoal(int32_t _targetPositionClicks = 0, int32_t _targetRelativeTimeMicros = 0) : targetPositionClicks(_targetPositionClicks), targetRelativeTimeMicros(_targetRelativeTimeMicros)
    {
    }
};

SerialStream &operator<<(SerialStream &stream, const MotorCommanderGoal &goal);

struct MotorSpeedCommanderInterface
{
    virtual void setGoal(MotorCommanderGoal command, int32_t nowMicros) = 0;
};
