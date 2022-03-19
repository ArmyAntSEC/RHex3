#pragma once

#include <HardwareInterrupts.h>
#include "HomingEncoderInterfaces.h"

class LinearPositionEncoder: public BasicEncoderListener, public LinearPositionProvider
{
private:
    volatile int32_t linearPosition = 0;
    volatile bool isHomed = false;
    HardwareInterruptsInterface* hwInterrupts;
public:
    LinearPositionEncoder( HardwareInterruptsInterface* _interrupts ): hwInterrupts(_interrupts)    
    { }

    virtual void signalStepForwardISR()
    {
        linearPosition++;
    }
    
    virtual void signalHomingISR()
    {
        if ( !isHomed )
            linearPosition = 0;
        isHomed = true;
    }

    virtual int32_t getLinearPosition()
    {
        int32_t rValue = 0;
        hwInterrupts->disableInterrupts();
        rValue = linearPosition;
        hwInterrupts->enableInterrupts();
        return rValue;
    }

    void forceHomed()
    {
        isHomed = true;
    }

};

