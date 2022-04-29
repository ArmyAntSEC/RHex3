#pragma once
#include <MotorSpeedRegulatorInterfaces.h>

class MotorSpeedCommanderSender: public MotorSpeedCommanderInterface
{
    virtual void setGoal(MotorCommanderGoalInterface* command, int32_t nowMicros )
    {
        
    }
};