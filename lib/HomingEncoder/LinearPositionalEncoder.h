#pragma once

#include "BasicEncoder.h"

struct LinearPositionProvider
{
    virtual long getLinearPosition() = 0;    
};

class LinearPositionEncoder: public BasicEncoderListener, LinearPositionProvider
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

    virtual long getLinearPosition()
    {
        long rValue = 0;
        HardwareInterface::disableInterrupts();
        rValue = linearPosition;
        HardwareInterface::enableInterrupts();
        return rValue;
    }

};

