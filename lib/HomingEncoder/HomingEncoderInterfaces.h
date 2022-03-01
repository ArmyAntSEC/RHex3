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

struct RotationalPositionProvider
{
    static const long clicksPerLapNum = 10775776;
    static const long clicksPerLapDen = 3000;

    virtual long getLaps() = 0;    
    virtual long getClicks() = 0;
};
