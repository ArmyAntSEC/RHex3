#pragma once

#include <HardwareInterrupts.h>
#include "HomingEncoderInterfaces.h"

class LinearPositionEncoder: public BasicEncoderListener, public LinearPositionProvider
{
private:
    volatile long linearPosition = 0;
    volatile bool isHomed = false;
    HardwareInterruptsInterface* hwInterrupts;
public:

    void config( HardwareInterruptsInterface* _interrupts )
    {
        hwInterrupts = _interrupts;
    }

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

    virtual long getLinearPosition()
    {
        long rValue = 0;
        hwInterrupts->disableInterrupts();
        rValue = linearPosition;
        hwInterrupts->enableInterrupts();
        return rValue;
    }

};

