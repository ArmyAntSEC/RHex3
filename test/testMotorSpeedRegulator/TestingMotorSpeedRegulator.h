#pragma once

#include <MotorSpeedRegulator.h>

struct TestingMotorSpeedRegulator: public MotorSpeedRegulator
{
    int countDoCorePIDAlgorithmStepClampedForSpeed = 0;
    int countHandleHardBreak = 0;
    
    virtual void handleHardBreak()
    {
        countHandleHardBreak++;
    }

    virtual void doCorePIDAlgorithmStepClampedForSpeed()
    {
        countDoCorePIDAlgorithmStepClampedForSpeed++;
    }

};