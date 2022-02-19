#pragma once

#include "BasicEncoder.h"
#include "SpeedometerInterface.h"

class CanProvideSpeed
{
    public:
        virtual int getSpeedCPS() = 0;
};

class SpeedComputer: public BasicEncoderListener, CanProvideSpeed
{
private:
    volatile long timeSinceLastStepUS = 0;
    volatile long speedCPS = 0;
public:
    virtual void signalStepForwardISR()
    {
        long thisTimeUS = HardwareInterface::getMicrosecondsSinceBoot();
        long timeDiff = thisTimeUS - timeSinceLastStepUS;
        speedCPS = 1e6 / timeDiff;
    }
    
    virtual int getSpeedCPS()
    {
        HardwareInterface::disableInterrupts();
        return speedCPS;
        HardwareInterface::enableInterrupts();
    }

    virtual void signalHomingISR()
    { /* Not used. Breaks Interface principle */ }

};