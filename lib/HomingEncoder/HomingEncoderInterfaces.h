#pragma once

class BasicEncoderListener
{
public:
    virtual void signalStepForwardISR() = 0;
    virtual void signalHomingISR() = 0;
};

struct LinearPositionProvider
{
    virtual long getLinearPosition() = 0;    
};

class CanProvideSpeed
{
    public:
        virtual int getSpeedCPS() = 0;
};
