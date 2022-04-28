#pragma once
#include <MotorSpeedRegulatorInterfaces.h>

class MotorSpeedCommanderSender: public MotorSpeedCommanderInterface
{
    virtual void setGoal(LegCommand) 
    {
        
    }
};