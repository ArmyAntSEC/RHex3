#pragma once

#include <BasicEncoder.h>

struct BasicEncoderListenerMock: public BasicEncoderListener
{
    volatile int32_t stepCounter = 0;
    volatile int32_t homingCounter = 0;

    void signalStepForwardISR() 
    {
        stepCounter++;
    }
    
    void signalHomingISR() 
    {
        homingCounter++;
    }
};
