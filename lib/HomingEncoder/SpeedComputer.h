#pragma once

#include "HomingEncoderInterfaces.h"
#include "HardwareClock.h"
#include "HardwareInterrupts.h"
#include <SerialStream.h>

class SpeedComputer: public CanProvideSpeed, public BasicEncoderListener 
{
private:
    volatile long timeAtLastUpdateMicros = 0;
    volatile long timeAtThisUpdateMicros = 0;
    volatile int16_t clicksSinceLastUpdate = 0;    

    HardwareClockInterface* hwClock;
    HardwareInterruptsInterface* hwInterrupts;

public:
    
    SpeedComputer( HardwareClockInterface* _clock, HardwareInterruptsInterface* _interrupts ):
        hwClock(_clock), hwInterrupts(_interrupts)
    {}

    virtual void signalStepForwardISR()
    {
        //We measure the speed every 10th click. Gives some filtering and also saves compute time.        
        if ( clicksSinceLastUpdate < 9 ) { //9 of these and one measurement becomes 10.
            clicksSinceLastUpdate++;
        } else {
            clicksSinceLastUpdate = 0;
            timeAtLastUpdateMicros = timeAtThisUpdateMicros;
            timeAtThisUpdateMicros = hwClock->getMicrosecondsSinceBoot();                
        }
        
    }
    
    virtual int16_t getSpeedCPS ( )
    {
        
        hwInterrupts->disableInterrupts();
        long lastTime = timeAtLastUpdateMicros;
        long thisTime = timeAtThisUpdateMicros;
        hwInterrupts->enableInterrupts();        
        long timeDiff = thisTime - lastTime;


        if ( timeDiff == 0 )                  
            return 0;
        else
            return 1e6L * 10L / timeDiff;  //We only measure every 10th click.   
    }

    virtual void signalHomingISR()
    { /* Not used. Breaks Interface principle */ }

};