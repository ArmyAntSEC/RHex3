#pragma once
#include <base.h>

class BasicEncoderListener
{
public:
    virtual void signalStepForwardISR() = 0;
    virtual void signalHomingISR() = 0;
};

struct LinearPositionProvider
{
    virtual int32_t getLinearPosition() = 0;    
    virtual bool isHomed() = 0;
    virtual void unHome() = 0;
};

class CanProvideSpeed
{
    public:
        virtual int16_t getSpeedCPS() = 0;
};