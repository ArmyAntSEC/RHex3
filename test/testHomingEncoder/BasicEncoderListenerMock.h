#pragma once

#include <BasicEncoder.h>

struct BasicEncoderListenerMock: public BasicEncoderListener
{
    volatile long stepCounter = 0;
    volatile long homingCounter = 0;

    void signalStepForwardISR() 
    {
        stepCounter++;
    }
    
    void signalHomingISR() 
    {
        homingCounter++;
    }
};
