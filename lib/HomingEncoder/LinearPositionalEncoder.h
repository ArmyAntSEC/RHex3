#pragma once

#include <HardwareInterrupts.h>
#include <HardwareInterface.h>
#include "HomingEncoderInterfaces.h"

class LinearPositionEncoder: public BasicEncoderListener, LinearPositionProvider
{
private:
    volatile long linearPosition = 0;
    volatile bool isHomed = false;
    HardwareInterrupts* hwInterrupts;
public:

    LinearPositionEncoder( HardwareInterrupts* _interrupts ): hwInterrupts(_interrupts)
    {}

    virtual void signalStepForwardISR()
    {
        linearPosition++;
    }
    
    virtual void signalHomingISR()
    {
        if ( !isHomed )
            linearPosition = 0;
    }

    virtual long getLinearPosition()
    {
        long rValue = 0;
        HardwareInterface::disableInterrupts();
        rValue = linearPosition;
        HardwareInterface::enableInterrupts();
        return rValue;
    }

};

