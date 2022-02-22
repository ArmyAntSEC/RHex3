#pragma once

#include "HomingEncoderInterfaces.h"
#include "HardwareClock.h"
#include "HardwareInterrupts.h"

class SpeedComputer: public BasicEncoderListener, CanProvideSpeed
{
private:
    volatile long timeSinceLastStepUS = 0;
    volatile long speedCPS = 0;
    HardwareClock* HWClock;
    HardwareInterrupts* HWInterrupts;

public:
    SpeedComputer( HardwareClock* _clock, HardwareInterrupts* _interrupts ): 
        HWClock(_clock), HWInterrupts(_interrupts)
    {}

    virtual void signalStepForwardISR()
    {
        long thisTimeUS = HWClock->getMicrosecondsSinceBoot();
        long timeDiff = thisTimeUS - timeSinceLastStepUS;
        speedCPS = 1e6 / timeDiff;
    }
    
    virtual int getSpeedCPS()
    {
        HWInterrupts->disableInterrupts();
        return speedCPS;
        HWInterrupts->enableInterrupts();
    }

    virtual void signalHomingISR()
    { /* Not used. Breaks Interface principle */ }

};