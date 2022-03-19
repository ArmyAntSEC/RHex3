#pragma once

#include "HomingEncoderInterfaces.h"
#include "HardwareClock.h"
#include "HardwareInterrupts.h"
#include <SerialStream.h>

class SpeedComputer: public CanProvideSpeed, public BasicEncoderListener 
{
private:
    volatile int32_t timeAtLastUpdateMicros = 0;
    volatile int32_t timeAtThisUpdateMicros = 0;
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
        int32_t lastTime = timeAtLastUpdateMicros;
        int32_t thisTime = timeAtThisUpdateMicros;
        hwInterrupts->enableInterrupts();        
        int32_t timeDiff = thisTime - lastTime;


        if ( timeDiff == 0 )                  
            return 0;
        else
            return 1e6L * 10L / timeDiff;  //We only measure every 10th click.   
    }

    virtual void signalHomingISR()
    { /* Not used. Breaks Interface principle */ }

};