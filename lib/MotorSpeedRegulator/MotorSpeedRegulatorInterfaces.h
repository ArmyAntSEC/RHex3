#pragma once

class MotorDriverInterface
{
    public:
        virtual void setMotorPWM( int motorPWM ) = 0;
        virtual int getMotorPWM() = 0;
};

struct MotorSpeedCommanderInterface
{
};

struct SpeedRegulatorInterface
{
    virtual void setSetPoint( int _setPoint ) = 0;
};
