#pragma once

#include "HomingEncoderInterfaces.h"
#include "HardwareClock.h"
#include "HardwareInterrupts.h"

class SpeedComputer: public BasicEncoderListener, CanProvideSpeed
{
private:
    volatile long timeSinceLastStepUS = 0;
    volatile long speedCPS = 0;
    HardwareClockInterface* hwClock;
    HardwareInterruptsInterface* hwInterrupts;

public:
    SpeedComputer( HardwareClockInterface* _clock, HardwareInterruptsInterface* _interrupts ): 
        hwClock(_clock), hwInterrupts(_interrupts)
    {}

    virtual void signalStepForwardISR()
    {
        long thisTimeUS = hwClock->getMicrosecondsSinceBoot();
        long timeDiff = thisTimeUS - timeSinceLastStepUS;
        speedCPS = 1e6 / timeDiff;
    }
    
    virtual int getSpeedCPS()
    {
        
        hwInterrupts->disableInterrupts();
        int rValue = speedCPS;
        hwInterrupts->enableInterrupts();
        return rValue;
    }

    virtual void signalHomingISR()
    { /* Not used. Breaks Interface principle */ }

};