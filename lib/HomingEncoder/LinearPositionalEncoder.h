#pragma once

#include "BasicEncoder.h"

class LinearPositionEncoder: public BasicEncoderListener
{
private:
    volatile long linearPosition = 0;
    volatile bool isHomed = false;
public:

    virtual void signalStepForwardISR()
    {
        linearPosition++;
    }
    
    virtual void signalHomingISR()
    {
        if ( !isHomed )
            linearPosition = 0;
    }

    long getLinearPosition()
    {
        long rValue = 0;
        HardwareInterface::disableInterrupts();
        rValue = linearPosition;
        HardwareInterface::enableInterrupts();
        return rValue;
    }

};

