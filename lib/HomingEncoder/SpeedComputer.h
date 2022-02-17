#pragma once

#include "BasicEncoder.h"

class SpeedComputer: public BasicEncoderListener 
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
    
    int getSpeedCPS()
    {
        HardwareInterface::disableInterrupts();
        return speedCPS;
        HardwareInterface::enableInterrupts();
    }

    virtual void signalHomingISR()
    { /* Not used. Breaks Interface SOLID principle */ }

};