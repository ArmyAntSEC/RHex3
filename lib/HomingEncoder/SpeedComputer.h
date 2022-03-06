#pragma once

#include "HomingEncoderInterfaces.h"
#include "HardwareClock.h"
#include "HardwareInterrupts.h"

class SpeedComputer: public BasicEncoderListener, CanProvideSpeed
{
private:
    volatile long timeAtLastClickMicros = 0;
    volatile long timeAtThisClickMicros = 0;

    HardwareClockInterface* hwClock;
    HardwareInterruptsInterface* hwInterrupts;

public:
    SpeedComputer( HardwareClockInterface* _clock, HardwareInterruptsInterface* _interrupts ): 
        hwClock(_clock), hwInterrupts(_interrupts)
    {}

    virtual void signalStepForwardISR()
    {
        timeAtLastClickMicros = timeAtThisClickMicros;
        timeAtThisClickMicros = hwClock->getMicrosecondsSinceBoot();                
    }
    
    virtual int getSpeedCPS()
    {
        
        hwInterrupts->disableInterrupts();
        long lastTime = timeAtLastClickMicros;
        long thisTime = timeAtThisClickMicros;
        hwInterrupts->enableInterrupts();
                
        long timeDiff = thisTime - lastTime;

        if ( timeDiff == 0 )                  
            return 0;
        else
            return 1e6 / timeDiff;        
    }

    virtual void signalHomingISR()
    { /* Not used. Breaks Interface principle */ }

};