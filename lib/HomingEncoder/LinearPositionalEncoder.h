#pragma once
#include <base.h>
#include <HardwareInterrupts.h>
#include "HomingEncoderInterfaces.h"

class LinearPositionEncoder: public BasicEncoderListener, public LinearPositionProvider
{
private:
    volatile int32_t linearPosition = 0;
    volatile bool encoderIsHomed = false;
    HardwareInterruptsInterface* hwInterrupts;
    int16_t offset = 0;

public:
    LinearPositionEncoder( HardwareInterruptsInterface* _interrupts ): hwInterrupts(_interrupts)    
    { }

    virtual void signalStepForwardISR()
    {
        linearPosition++;
    }
    
    virtual void signalHomingISR()
    {
        if ( !encoderIsHomed )
            linearPosition = 0;
        encoderIsHomed = true;
    }

    virtual int32_t getLinearPosition()
    {
        int32_t rValue = 0;
        hwInterrupts->disableInterrupts();
        rValue = linearPosition;
        hwInterrupts->enableInterrupts();
        return rValue - offset;
    }

    virtual bool isHomed()
    {
        return encoderIsHomed;
    }

    virtual void unHome()
    {
        encoderIsHomed = false;
    }

    void forceHomed()
    {
        encoderIsHomed = true;
    }

    void setOffset( int16_t _offset )
    {        
        offset = _offset;
    }

};

